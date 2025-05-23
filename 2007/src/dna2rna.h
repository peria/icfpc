#ifndef DNA2RNA_H_
#define DNA2RNA_H_

#include <ext/rope>
#include <vector>

using String = __gnu_cxx::crope;
using Dna = String;

struct PItem {
  enum Type { kBase, kSkip, kSearch, kBla, kCket };

  Type type;
  char base;
  int n;
  String s;

  PItem(const char b) : type(kBase), base(b) {}
  PItem(const int m) : type(kSkip), n(m) {}
  PItem(const String& s) : type(kSearch), s(s) {}
  PItem(const Type t) : type(t) {}
};

using Pattern = std::vector<PItem>;

struct TItem {
  enum Type { kBase, kReference, kLength };

  Type type;
  char base;
  size_t number;
  size_t level;

  TItem(const char b) : type(kBase), base(b) {}
  TItem(const int n, const int l) : type(kReference), number(n), level(l) {}
  TItem(const int n) : type(kLength), number(n) {}
};
using Template = std::vector<TItem>;
using Environment = std::vector<Dna>;

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
  Pattern MakePattern();
  Template MakeTemplate();
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
