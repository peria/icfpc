#include <iostream>
#include <memory>

#include "ai/ai.h"
#include "core/communicator.h"

int main() {
  // Create AI factory
  Communicator communicator(std::cin, std::cout);
  communicator.messageLoop();

  return 0;
}
