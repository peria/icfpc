#include "dna2rna.h"

#include <ext/rope>
#include <iostream>
#include <string>

int main(void) {
  std::string dna;
  std::cin >> dna;
  Dna2Rna endo(dna.c_str());
  for (const Dna& rna : endo.Execute())
    std::cout << rna << "\n";

  return 0;
}
