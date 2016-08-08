#pragma once

#include <iostream>

#include "base.h"
#include "json.h"

class Communicator {
 public:
  Communicator(std::istream& ist, std::ostream& ost);
  void messageLoop();

 protected:
  const Json recieveMessage();
  int getMessageSize();

  std::istream& ist;
  std::ostream& ost;
};
