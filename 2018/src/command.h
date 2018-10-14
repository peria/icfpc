#pragma once

#include <ostream>

#include "coordinate.h"

struct Command {
  enum Type {
    kHalt,
    kWait,
    kFlip,
    kSMove,
    kLMove,
    kFission,
    kFill,
    kVoid,
    kFusionP,
    kFusionS,
    kGFill,
    kGVoid,
    kSync,
  };

  template <typename T>
  const T* To() const {
    return static_cast<const T*>(this);
  }

  virtual bool needSort() const { return false; }
  virtual ~Command() = default;
  virtual Type type() const = 0;
};

struct Halt : public Command {
  ~Halt() override {}
  Type type() const override { return kHalt; }
};

struct Wait : public Command {
  ~Wait() override {}
  Type type() const override { return kWait; }
};

struct Flip : public Command {
  ~Flip() override {}
  Type type() const override { return kFlip; }
};

struct SMove : public Command {
  SMove(const LLD& c) : lld(c) {}
  ~SMove() override {}
  Type type() const override { return kSMove; }

  const LLD lld;
};

struct LMove : public Command {
  LMove(const SLD& c1, const SLD& c2) : sld1(c1), sld2(c2) {}
  ~LMove() override {}
  Type type() const override { return kLMove; }

  const SLD sld1;
  const SLD sld2;
};

struct Fission : public Command {
  Fission(const ND& nd_, int m_) : nd(nd_), m(m_) {}
  ~Fission() override {}
  Type type() const override { return kFission; }
  bool needSort() const override { return true; }

  const ND nd;
  const int m;
};

struct Fill : public Command {
  Fill(const ND& nd_) : nd(nd_) {}
  ~Fill() override {}
  Type type() const override { return kFill; }

  const ND nd;
};

struct Void : public Command {
  Void(const ND& nd_) : nd(nd_) {}
  ~Void() override {}
  Type type() const override { return kVoid; }

  const ND nd;
};

struct FusionP : public Command {
  FusionP(const ND& nd_) : nd(nd_) {}
  ~FusionP() override {}
  Type type() const override { return kFusionP; }
  bool needSort() const override { return true; }

  const ND nd;
};

struct FusionS : public Command {
  FusionS(const ND& nd_) : nd(nd_) {}
  ~FusionS() override {}
  Type type() const override { return kFusionS; }
  bool needSort() const override { return true; }

  const ND nd;
};

struct GFill : public Command {
  GFill(const ND& nd, const FD& fd) : nd(nd), fd(fd) {}
  ~GFill() override {}
  Type type() const override { return kGFill; }

  const ND nd;
  const FD fd;
};

struct GVoid : public Command {
  GVoid(const ND& nd, const FD& fd) : nd(nd), fd(fd) {}
  ~GVoid() override {}
  Type type() const override { return kGVoid; }

  const ND nd;
  const FD fd;
};

struct Sync : public Command {
  ~Sync() override {}
  Type type() const override { return kSync; }
};

std::ostream& operator<<(std::ostream&, const Command&);
