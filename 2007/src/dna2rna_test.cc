#include "dna2rna.h"

#include <gtest/gtest.h>

#include <ext/rope>
#include <string>
#include <vector>

class TDna2Rna : public Dna2Rna {
 public:
  TDna2Rna(const String& dna) : Dna2Rna(dna) {}
  std::string dna() { return dna_.c_str() + i_; }

  using Dna2Rna::Consts;
  using Dna2Rna::ExecuteIteration;
  using Dna2Rna::i_;
  using Dna2Rna::MakePattern;
  using Dna2Rna::Nat;
  using Dna2Rna::Protect;
};

class Dna2RnaTest : public testing::Test {};

TEST_F(Dna2RnaTest, Pattern) {
  {
    TDna2Rna endo("CIIC");
    Pattern pat;
    endo.MakePattern(&pat);
    ASSERT_EQ(1, pat.size());
    EXPECT_EQ(PItem::BASE, pat[0].type);
    EXPECT_EQ('I', pat[0].base);
  }
  {
    TDna2Rna endo("IIPIPICPIICICIIF");
    Pattern pat;
    endo.MakePattern(&pat);
    ASSERT_EQ(4, pat.size());
    EXPECT_EQ(PItem::BLA, pat[0].type);
    EXPECT_EQ(PItem::SKIP, pat[1].type);
    EXPECT_EQ(2, pat[1].n);
    EXPECT_EQ(PItem::CKET, pat[2].type);
    EXPECT_EQ(PItem::BASE, pat[3].type);
    EXPECT_EQ('P', pat[3].base);
  }
}

TEST_F(Dna2RnaTest, Protect) {
  EXPECT_EQ("CF", std::string(TDna2Rna::Protect(0, "CF").c_str()));
  EXPECT_EQ("ICCFCFFP", std::string(TDna2Rna::Protect(7, "CF").c_str()));
}

TEST_F(Dna2RnaTest, Nat) {
  struct TestData {
    const char* dna;
    int expect;
    const char* remain;
  } tests[] = {
      {"P", 0, ""},        {"CP", 1, ""},          {"ICP", 2, ""},
      {"ICFP", 2, ""},     {"CCCCP", 15, ""},      {"PCCP", 0, "CCP"},
      {"CPICF", 1, "ICF"}, {"ICPICFP", 2, "ICFP"},
  };
  for (const TestData& test : tests) {
    TDna2Rna endo(test.dna);
    EXPECT_EQ(test.expect, endo.Nat()) << "Input: " << test.dna;
    EXPECT_EQ(test.remain, endo.dna()) << "Input: " << test.dna;
  }
}

TEST_F(Dna2RnaTest, Consts) {
  struct TestData {
    const char* dna;
    const char* expect;
    const char* remain;
  } tests[] = {
      {"", "", ""},   {"I", "", "I"},  {"C", "I", ""},   {"F", "C", ""},
      {"P", "F", ""}, {"IC", "P", ""}, {"II", "", "II"},
  };
  for (const TestData& test : tests) {
    TDna2Rna endo(test.dna);
    EXPECT_EQ(test.expect, std::string(endo.Consts().c_str()))
        << "Input: " << test.dna;
    EXPECT_EQ(test.remain, endo.dna()) << "Input: " << test.dna;
  }
}

TEST_F(Dna2RnaTest, ExecuteIteration) {
  struct TestData {
    const char* dna;
    const char* expect;
  } tests[] = {
      {"IIPIPICPIICICIIFICCIFPPIICCFPC", "PICFC"},
      {"IIPIPICPIICICIIFICCIFCCCPPIICCFPC", "PIICCFCFFPC"},
      {"IIPIPIICPIICIICCIICFCFC", "I"},
  };
  for (const TestData& test : tests) {
    TDna2Rna endo(test.dna);
    endo.ExecuteIteration();
    EXPECT_EQ(test.expect, endo.dna()) << "Input: " << test.dna;
  }
}

TEST_F(Dna2RnaTest, Asnat) {
  struct TestData {
    int n;
    const char* expect;
  } tests[] = {
      {0, "P"},
      {1, "CP"},
      {2, "ICP"},
      {5, "CICP"},
  };

  for (const TestData& test : tests) {
    EXPECT_EQ(std::string(test.expect), Dna2Rna::Asnat(test.n).c_str());
  }
}
