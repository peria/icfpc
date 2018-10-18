#include "dna2rna.h"

#include <glog/logging.h>

#include <ext/rope>
#include <string>
#include <vector>

Dna2Rna::Dna2Rna(const String& dna) : dna_(dna), i_(0) {}

const std::vector<Dna>& Dna2Rna::Execute() {
  try {
    while (i_ < dna_.size())
      ExecuteIteration();
  } catch (const std::vector<Dna>& rna) {
    return rna_;
  }
  return rna_;
}

void Dna2Rna::Finish() {
  throw rna_;
}

void Dna2Rna::ExecuteIteration() {
  Pattern pattern = MakePattern();
  Template tmpl = MakeTemplate();
  MatchReplace(pattern, tmpl);
}

Pattern Dna2Rna::MakePattern() {
  Pattern pattern;

  int lvl = 0;
  while (i_ < dna_.size()) {
    switch (dna_[i_]) {
    case 'C': {
      ++i_;
      pattern.emplace_back('I');
      break;
    }
    case 'F': {
      ++i_;
      pattern.emplace_back('C');
      break;
    }
    case 'P': {
      ++i_;
      pattern.emplace_back('F');
      break;
    }
    case 'I': {
      if (i_ + 1 >= dna_.size())
        Finish();

      switch (dna_[i_ + 1]) {
      case 'C': {
        i_ += 2;
        pattern.emplace_back('P');
        break;
      }
      case 'P': {
        i_ += 2;
        int n = Nat();
        pattern.emplace_back(n);
        break;
      }
      case 'F': {
        i_ += 3;
        const Dna& s = Consts();
        pattern.emplace_back(s);
        break;
      }
      case 'I': {
        if (i_ + 2 >= dna_.size())
          Finish();

        switch (dna_[i_ + 2]) {
        case 'P': {
          i_ += 3;
          ++lvl;
          pattern.emplace_back(PItem::kBla);
          break;
        }
        case 'C':
        case 'F': {
          i_ += 3;
          if (lvl == 0)
            return std::move(pattern);
          --lvl;
          pattern.emplace_back(PItem::kCket);
          break;
        }
        case 'I': {
          rna_.push_back(dna_.substr(i_ + 3, 7));
          i_ += 10;
          break;
        }
        default:
          Finish();
        }  // switch([i+2])
        break;
      }
      default:
        Finish();
      }  // switch([i+1])
      break;
    }
    default:
      Finish();
    }  // switch([i])
  }    // while()

  return std::move(pattern);
}

Template Dna2Rna::MakeTemplate() {
  Template tmpl;

  while (i_ < dna_.size()) {
    switch (dna_[i_]) {
    case 'C':
      ++i_;
      tmpl.emplace_back('I');
      break;
    case 'F':
      ++i_;
      tmpl.emplace_back('C');
      break;
    case 'P':
      ++i_;
      tmpl.emplace_back('F');
      break;
    case 'I': {
      if (i_ + 1 >= dna_.size())
        Finish();

      switch (dna_[i_ + 1]) {
      case 'C':
        i_ += 2;
        tmpl.emplace_back('P');
        break;
      case 'F':
      case 'P': {
        i_ += 2;
        int l = Nat();
        int n = Nat();
        tmpl.emplace_back(n, l);
        break;
      }
      case 'I': {
        if (i_ + 2 >= dna_.size())
          Finish();

        switch (dna_[i_ + 2]) {
        case 'C':
        case 'F': {
          i_ += 3;
          return std::move(tmpl);
        }
        case 'P': {
          i_ += 3;
          int n = Nat();
          tmpl.emplace_back(n);
          break;
        }
        case 'I': {
          rna_.push_back(dna_.substr(i_ + 3, 7));
          i_ += 10;
          break;
        }
        default:
          Finish();
        }  // switch ([i+2])
        break;
      }
      default:
        Finish();
      }  // switch ([i+1])
      break;
    }
    default:
      Finish();
    }  // switch ([i])
  }

  return std::move(tmpl);
}

void Dna2Rna::MatchReplace(const Pattern& pat, const Template& t) {
  size_t i = i_;
  Environment e;
  std::vector<int> c;
  for (const PItem& p : pat) {
    switch (p.type) {
    case PItem::kBase: {
      if (dna_[i] != p.base)
        return;
      ++i;
      break;
    }
    case PItem::kSkip: {
      i += p.n;
      if (i > dna_.size())
        return;
      break;
    }
    case PItem::kSearch: {
      size_t n = dna_.find(p.sequence.c_str(), i);
      if (n == std::string::npos)
        return;
      i = n + p.sequence.size();
      break;
    }
    case PItem::kBla: {
      c.push_back(i);
      break;
    }
    case PItem::kCket: {
      int c0 = c.back();
      e.push_back(dna_.substr(c0, i - c0));
      c.pop_back();
      break;
    }
    }
  }
  i_ = i;
  Replace(t, e);
}

int Dna2Rna::Nat() {
  if (i_ >= dna_.size())
    Finish();

  int nat = 0;
  int base = 1;
  while (i_ < dna_.size()) {
    switch (dna_[i_]) {
    case 'I':
    case 'F':
      ++i_;
      break;
    case 'C':
      ++i_;
      nat += base;
      break;
    case 'P':
      ++i_;
      return nat;
    default:
      Finish();
    }
    base *= 2;
  }

  return nat;
}

Dna Dna2Rna::Consts() {
  Dna consts;
  while (i_ < dna_.size()) {
    if (dna_[i_] == 'I' && i_ + 1 < dna_.size() && dna_[i_ + 1] == 'C') {
      i_ += 2;
      consts.push_back('P');
      continue;
    }
    switch (dna_[i_]) {
    case 'C': {
      ++i_;
      consts.push_back('I');
      continue;
    }
    case 'F': {
      ++i_;
      consts.push_back('C');
      continue;
    }
    case 'P': {
      ++i_;
      consts.push_back('F');
      continue;
    }
    }
    break;
  }
  return consts;
}

void Dna2Rna::Replace(const Template& tpl, const Environment& e) {
  Dna r;
  for (const TItem& t : tpl) {
    switch (t.type) {
    case TItem::kBase: {
      r.push_back(t.base);
      break;
    }
    case TItem::kReference: {
      if (t.number >= e.size())
        r.append(Protect(t.level, ""));
      else
        r.append(Protect(t.level, e[t.number]));
      break;
    }
    case TItem::kLength: {
      if (t.number >= e.size())
        r.append(Asnat(0));
      else
        r.append(Asnat(e[t.number].size()));
      break;
    }
    }
  }

  dna_.erase(0, i_);
  dna_.insert(0, r);
  i_ = 0;
}

Dna Dna2Rna::Protect(int l, const Dna& d) {
  if (d.empty())
    return "";

  Dna protect(d);
  for (int i = 0; i < l; ++i)
    protect = Quote(protect);
  return protect;
}

Dna Dna2Rna::Quote(const Dna& d) {
  Dna quote;
  for (size_t i = 0; i < d.size(); ++i) {
    switch (d[i]) {
    case 'I': {
      quote.push_back('C');
      break;
    }
    case 'C': {
      quote.push_back('F');
      break;
    }
    case 'F': {
      quote.push_back('P');
      break;
    }
    case 'P': {
      quote.append("IC");
      break;
    }
    default:
      return quote;
    }
  }
  return quote;
}

Dna Dna2Rna::Asnat(int n) {
  Dna asnat;
  for (; n; n /= 2)
    asnat.push_back((n & 1) ? 'C' : 'I');
  asnat.push_back('P');
  return asnat;
}
