#include <iostream>
#include <string>
#include <vector>

#include "base.h"
#include "expression.h"
#include "problem.h"

int main(void) {
  // Input
  int size;
  int num_operators;
  uint32 operators = 0;

  std::cin >> size;
  std::cin >> num_operators;
  for (int i = 0; i <  num_operators; ++i) {
    std::string str;
    std::cin >> str;
    if (str == "not")
      operators |= NOT;
    else if (str == "shl1")
      operators |= SHL1;
    else if (str == "shr1")
      operators |= SHR1;
    else if (str == "shr4")
      operators |= SHR4;
    else if (str == "shr16")
      operators |= SHR16;
    else if (str == "and")
      operators |= AND;
    else if (str == "or")
      operators |= OR;
    else if (str == "xor")
      operators |= XOR;
    else if (str == "plus")
      operators |= PLUS;
    else if (str == "if0")
      operators |= IF0;
    else if (str == "fold")
      operators |= FOLD;
    else if (str == "tfold")
      operators |= TFOLD;
  }
  Problem problem(size, operators);

  int num_ios;
  std::cin >> num_ios;
  std::vector<uint64> inputs(num_ios);
  std::vector<uint64> expects(num_ios);
  for (int i = 0; i < num_ios; ++i)
    std::cin >> inputs[i] >> expects[i];

  while (true) {
    const std::string program = problem.GetNextProgram(inputs, expects);
    if (program == "") {
      std::cerr << "No valid programs are found.\n";
      break;
    }

    std::cout << program << std::endl;
    break;
    if (std::cin.eof()) {
      std::cerr << "Find a valid program\n" << program << std::endl;
      break;
    }

    // Input a counter example.
    uint64 input;
    uint64 expect;
    std::cin >> input >> expect;
    inputs.push_back(input);
    expects.push_back(expect);
  }

  return 0;
}
