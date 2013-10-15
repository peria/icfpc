#ifndef DNA2RNA_H_
#define DNA2RNA_H_

#include <ext/rope>
#include <vector>

typedef __gnu_cxx::crope String;
typedef String Dna;

struct PItem {
  enum Type { BASE, SKIP, SEARCH, BLA, CKET } type;
  char base;
  int n;
  String sequence;

  PItem(const char b) : type(BASE), base(b) {}
  PItem(const int m) : type(SKIP), n(m) {}
  PItem(const String& s) : type(SEARCH), sequence(s) {}
  PItem(const Type t) : type(t) {}
};
typedef std::vector<PItem> Pattern;

struct TItem {
  enum Type { BASE, REFERENCE, LENGTH } type;
  char base;
  size_t number;
  size_t level;

  TItem(const char b) : type(BASE), base(b) {}
  TItem(const int n, const int l) : type(REFERENCE), number(n), level(l) {}
  TItem(const int n) : type(LENGTH), number(n) {}
};
typedef std::vector<TItem> Template;

typedef std::vector<Dna> Environment;

class Dna2Rna {
 public:
  Dna2Rna(const String& dna);
  const std::vector<Dna>& Execute();

  static Dna Protect(int l, const Dna& d);
  static Dna Quote(const Dna& d);
  static Dna Asnat(int n);

 protected:
  void Finish();
  void ExecuteIteration();
  void MakePattern(Pattern* p);
  void MakeTemplate(Template* t);
  void MatchReplace(const Pattern& p, const Template& t);
  int Nat();
  String Consts();
  void Replace(const Template& t, const Environment& e);
  const char* dna() { return dna_.substr(i_).c_str(); }

  Dna dna_;
  std::vector<Dna> rna_;
  size_t i_;
};

#endif  // DNA2RNA_H_
