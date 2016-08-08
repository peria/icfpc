#include "client.h"

#include <string>
#include <vector>

#include <glog/logging.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>

#include "core/punter.h"

using std::string;


Client::Client(const char* path, Punter& punter, Game& game)
    : exe_path_(path),
      punter_(punter),
      game_(game),
      read_fd_(-1),
      write_fd_(-1) {}

// TODO: Move this phase into (offline|online) server.
Json Client::communicate() {
  if (punter().state() == Punter::State::Zombie) {
    return messagePass();
  }

  launch();
  shakeHands();
  Json message = messagePass();
  switch (punter().state()) {
    case Punter::State::Setup: {
      setTimeout(10);
      sendMessage(
          {
            {"punter", punter().punter_id()},
            {"punters", game_.num_punters()},
            {"map", game_.map().serialize()}
          });
      message = recvMessage();
      break;
    }
    case Punter::State::Gameplay: {
      setTimeout(1);
      sendMessage({});
      message = recvMessage();
      break;
    }
    case Punter::State::Scoring:
    case Punter::State::Zombie: {
      // not reached
      break;
    }
  }
  return message;
}

void Client::reportScores(const Json& scores) {
  if (punter().state() == Punter::State::Zombie) {
    return;
  }

  launch();
  shakeHands();
  Json message = {
    "stop", {
      {"moves", moves_},
      {"scores", scores}
    },
  };
  sendMessage(message);
}

bool Client::launch() {
  static const int kReadIndex = 0;
  static const int kWriteIndex = 1;

  if (isZombie())
    return false;
  int pipe_c2p[2];
  int pipe_p2c[2];

  // Create pipes
  pipe(pipe_c2p);
  pipe(pipe_p2c);

  pid_ = fork();
  if (pid_ == 0) {
    // Child process
    close(pipe_p2c[kWriteIndex]);
    close(pipe_c2p[kReadIndex]);
    dup2(pipe_p2c[kReadIndex], STDIN_FILENO);
    dup2(pipe_c2p[kWriteIndex], STDOUT_FILENO);
    close(pipe_p2c[kReadIndex]);
    close(pipe_c2p[kWriteIndex]);
    char* argv[] = {nullptr};
    execvp(exe_path_, argv);
    // NOTREACHED();
  }

  // Parent process
  close(pipe_p2c[kReadIndex]);
  close(pipe_c2p[kWriteIndex]);
  write_fd_ = pipe_p2c[kWriteIndex];
  read_fd_ = pipe_c2p[kReadIndex];

  return true;
}

bool Client::shakeHands() {
  if (isZombie())
    return false;

  // P->S: {"me": name}
  Json message = recvMessage();
  if (message.find("me") == message.end())
    return false;

  // P<-S: {"you": name}
  string name = message["me"];
  punter().set_name(name);
  return sendMessage({"you", name});
}

bool Client::setTimeout(int time_sec) {
  if (isZombie())
    return false;
  // TODO: Implement timeout
  return true;
}

bool Client::sendMessage(const Json& message) {
  if (isZombie())
    return false;
  if (write_fd_ < 0)
    return false;

  // Compute size prefix.
  string str = message.dump();
  int size = str.size();
  std::ostringstream oss;
  oss << size << ":";

  // TODO: Check if whole message is passed.
  write(write_fd_, oss.str().data(), oss.str().size());
  write(write_fd_, str.data(), str.size());
  return true;
}

Json Client::recvMessage() {
  if (isZombie()) {
    return messagePass();
  }

  int size = getMessageSize();
  Json json;
  if (size < 0) {
    // Failed to parse size.
    return json;
  }
  std::vector<char> json_str(size + 1);  // +1 for '\0'
  ::read(read_fd_, json_str.data(), size);
  return Json::parse(json_str);
}

bool Client::terminate() {
  if (isZombie()) {
    return false;
  }
  // Handling no processes.
  if (pid_ < 0) {
    return false;
  }

  kill(pid_, SIGKILL);
  int status;
  waitpid(pid_, &status, 0);

  pid_ = -1;
  read_fd_ = -1;
  write_fd_ = -1;
  return true;
}

Json Client::messagePass(const Json& state) const {
  Json json = {"pass", {"punter", punter().punter_id()}};
  if (!state.is_null()) {
    json["state"] = state;
  }
  return json;
}

int Client::getMessageSize() const {
  if (read_fd_ < 0)
    return -1;

  int size = 0;
  for (char c; read(read_fd_, &c, 1) == 1;) {
    if (c == ':')
      break;
    DCHECK(std::isdigit(c));
    size = size * 10 + (c - '0');
  }
  return size;
}
