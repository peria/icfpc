#include "dna2rna.h"

#include <glog/logging.h>

#include <fstream>
#include <iostream>
#include <string>

#ifndef ENDO_PATH
#define ENDO_PATH "../data/endo.dna"
#endif

int main(void) {
  std::string dna;
  std::cin >> dna;

  {
    std::ifstream ifs(ENDO_PATH);
    std::string endo;
    ifs >> endo;
    ifs.close();
    dna += endo;
  }
  LOG(INFO) << "Total length is " << dna.size() << " bytes";
  Dna2Rna endo(dna.c_str());
  for (const Dna& rna : endo.Execute())
    std::cout << rna << "\n";

  return 0;
}
