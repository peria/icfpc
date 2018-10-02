#pragma once

#include "coordinate.h"

#define REPEAT_COMMANDS(X) \
  X(Halt)                  \
  X(Wait)                  \
  X(Flip)                  \
  X(SMove)                 \
  X(LMove)                 \
  X(Fission)               \
  X(Fill)                  \
  X(Void)                  \
  X(FusionP)               \
  X(FusionS)               \
  X(GFill)                 \
  X(GVoid)                 \
  X(Sync)

#define FORWARD_DECLARE(A) struct A;
REPEAT_COMMANDS(FORWARD_DECLARE)
#undef FORAED_DECLARE

struct Command {
  enum Type {
#define ENUMERATE(A) k##A,
    REPEAT_COMMANDS(ENUMERATE)
#undef ENUMERATE
  };

#define CAST(A) inline const A* to##A() const;
  REPEAT_COMMANDS(CAST)
#undef CAST

  virtual ~Command() {}
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
  LMove(const SLD& c1, const SLD& c2)
      : sld1(c1), sld2(c2) {}
  ~LMove() override {}
  Type type() const override { return kLMove; }

  const SLD sld1;
  const SLD sld2;
};

struct Fission : public Command {
  Fission(const ND& nd_, int m_) : nd(nd_), m(m_) {}
  ~Fission() override {}
  Type type() const override { return kFission; }

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

  const ND nd;
};

struct FusionS : public Command {
  FusionS(const ND& nd_) : nd(nd_) {}
  ~FusionS() override {}
  Type type() const override { return kFusionS; }

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

#define CAST_IMPL(A)                      \
  const A* Command::to##A() const {       \
    return static_cast<const A*>(this);   \
  }
REPEAT_COMMANDS(CAST_IMPL)
#undef CAST_IMPL
