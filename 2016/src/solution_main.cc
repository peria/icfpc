#include <iostream>
#include <fstream>
#include <gmpxx.h>

#include "base.h"
#include "problem.h"
#include "vertex.h"
using namespace std;

int main(int argc, char* argv[]) {
  Problem problem;
  if (argc < 2) {
    cin >> problem;
  } else {
    ifstream ifs(argv[1]);
    ifs >> problem;
  }
  cout << problem.solve();
  return 0;
}
