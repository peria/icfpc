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
  // Take only the first line (first continuous string).
  // Other lines can be used to put comments. :)
  ifs >> original;
  ifs.close();

  std::string dna(prefix + original);
  LOG(INFO) << "Processing prefix:\n" << prefix;
  Dna2Rna endo(dna.c_str());
  for (const Dna& rna : endo.Execute())
    std::cout << rna << "\n";

  return 0;
}
