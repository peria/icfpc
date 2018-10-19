#include "dna2rna.h"

#include <glog/logging.h>

#include <fstream>
#include <iostream>
#include <string>

#ifndef ENDO_PATH
#define ENDO_PATH "../data/endo.dna"
#endif

int main(void) {
  std::string prefix;
  std::cin >> prefix;

  std::ifstream ifs(ENDO_PATH);
  std::string original;
  ifs >> original;
  ifs.close();

  std::string dna(prefix + original);
  LOG(INFO) << "Processing prefix:\n" << prefix;
  Dna2Rna endo(dna.c_str());
  for (const Dna& rna : endo.Execute())
    std::cout << rna << "\n";

  return 0;
}
