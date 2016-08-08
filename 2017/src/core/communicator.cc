#include "communicator.h"

#include <cctype>

#include "base.h"
#include "json.h"

Communicator::Communicator(std::istream& ist, std::ostream& ost)
    : ist(ist), ost(ost) {
}

void Communicator::messageLoop() {
  while (true) {
    const Json& json = recieveMessage();
    std::cerr << json.dump() << std::endl;
  }
}

const Json Communicator::recieveMessage() {
  int size = getMessageSize();
  Json json;
  if (size < 0) {
    std::cerr << "Failed to parse size." << std::endl;
    return json;
  }
  std::vector<char> json_str(size + 1);  // +1 for '\0'
  ist.read(json_str.data(), size);
  return Json::parse(json_str);
}

int Communicator::getMessageSize() {
  int size = 0;
  for (char c; ist >> c;) {
    if (c == ':')
      break;
    std::cerr << "c: " << c << "\n";
    CHECK(std::isdigit(c));
    size = size * 10 + (c - '0');
  }
  return size;
}
