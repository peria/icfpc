#include "communicator.h"

#include <sstream>
#include <iostream>
#include <gtest/gtest.h>

#include "json.h"

using std::istringstream;
using std::ostringstream;

class TestableCommunicator : public Communicator {
 public:
  TestableCommunicator(std::istream& ist, std::ostream& ost)
      : Communicator(ist, ost) {}

  using Communicator::recieveMessage;
  using Communicator::getMessageSize;
};

class CommunicatorTest : public testing::Test {
 public:
  void TearDown() {
    iss.str("");
    oss.clear();
  }

  istringstream iss;
  ostringstream oss;
};

TEST_F(CommunicatorTest, recieveMessageTest) {
  iss.str(R"(14:{"you":"name"})");
  TestableCommunicator communicator(iss, oss);
  const Json& json = communicator.recieveMessage();
  EXPECT_FALSE(json.is_null());
  EXPECT_NE(json.find("you"), json.end());
  EXPECT_EQ("name", json["you"]);
}
