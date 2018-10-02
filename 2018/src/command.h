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

#define FORWARD_DECLARE(A) struct A##Command;
REPEAT_COMMANDS(FORWARD_DECLARE)
#undef FORAED_DECLARE

struct Command {
  enum Type {
#define ENUMERATE(A) k##A,
    REPEAT_COMMANDS(ENUMERATE)
#undef ENUMERATE
  };

#define CAST(A) inline const A##Command* to##A() const;
  REPEAT_COMMANDS(CAST)
#undef CAST

  virtual ~Command();
  virtual Type type() const = 0;
};

struct HaltCommand : public Command {
  ~HaltCommand() override {}
  Type type() const override { return kHalt; }
};

struct WaitCommand : public Command {
  ~WaitCommand() override {}
  Type type() const override { return kWait; }
};

struct FlipCommand : public Command {
  ~FlipCommand() override {}
  Type type() const override { return kFlip; }
};

struct SMoveCommand : public Command {
  SMoveCommand(const Coordinate& c) : lld(c) { DCHECK(lld.isLLD()); }
  ~SMoveCommand() override {}
  Type type() const override { return kSMove; }

  const Coordinate lld;
};

struct LMoveCommand : public Command {
  LMoveCommand(const Coordinate& c1, const Coordinate& c2)
      : sld1(c1), sld2(c2) {
    DCHECK(sld1.isSLD());
    DCHECK(sld2.isSLD());
  }
  ~LMoveCommand() override {}
  Type type() const override { return kLMove; }

  const Coordinate sld1;
  const Coordinate sld2;
};

struct FissionCommand : public Command {
  FissionCommand(const Coordinate& nd_, int m_) : nd(nd_), m(m_) {
    DCHECK(nd.isNCD());
  }
  ~FissionCommand() override {}
  Type type() const override { return kFission; }

  const Coordinate nd;
  const int m;
};

struct FillCommand : public Command {
  FillCommand(const Coordinate& nd_) : nd(nd_) { DCHECK(nd.isNCD()); }
  ~FillCommand() override {}
  Type type() const override { return kFill; }

  const Coordinate nd;
};

struct VoidCommand : public Command {
  VoidCommand(const Coordinate& nd_) : nd(nd_) { DCHECK(nd.isNCD()); }
  ~VoidCommand() override {}
  Type type() const override { return kVoid; }

  const Coordinate nd;
};

struct FusionPCommand : public Command {
  FusionPCommand(const Coordinate& nd_) : nd(nd_) { DCHECK(nd.isNCD()); }
  ~FusionPCommand() override {}
  Type type() const override { return kFusionP; }

  const Coordinate nd;
};

struct FusionSCommand : public Command {
  FusionSCommand(const Coordinate& nd_) : nd(nd_) { DCHECK(nd.isNCD()); }
  ~FusionSCommand() override {}
  Type type() const override { return kFusionS; }

  const Coordinate nd;
};

struct GFillCommand : public Command {
  GFillCommand(const Coordinate& n, const Coordinate& f) : nd(n), fd(f) {
    DCHECK(nd.isNCD());
    DCHECK(fd.isFCD());
  }
  ~GFillCommand() override {}
  Type type() const override { return kGFill; }

  const Coordinate nd;
  const Coordinate fd;
};

struct GVoidCommand : public Command {
  GVoidCommand(const Coordinate& n, const Coordinate& f) : nd(n), fd(f) {
    DCHECK(nd.isNCD());
    DCHECK(fd.isFCD());
  }
  ~GVoidCommand() override {}
  Type type() const override { return kGVoid; }

  const Coordinate nd;
  const Coordinate fd;
};

struct SyncCommand : public Command {
  ~SyncCommand() override {}
  Type type() const override { return kSync; }
};

#define CAST_IMPL(A)                             \
  const A##Command* Command::to##A() const {     \
    return static_cast<const A##Command*>(this); \
  }
REPEAT_COMMANDS(CAST_IMPL)
#undef CAST_IMPL
