#pragma once

#include <cstdint>
#include <optional>
#include <string>

#include "pointer.h"

struct Expr {
  static Pointer<Expr> Create(const std::string& token);
  static Pointer<Expr> Create(int64_t number);

  virtual std::string ToString() const;

  virtual bool isAp() const { return false; }
  virtual bool isAtom() const { return false; }
  virtual bool isNumber() const { return false; }

  Pointer<Expr> evaluated;

 protected:
  Expr() = default;
};

struct Atom final : public Expr {
  explicit Atom(const std::string& name_) : name(name_) {}
  std::string ToString() const override { return name; }
  bool isAtom() const override { return true; }
  bool hasAlias() const { return alias.has_value(); }

  const std::string name;
  std::optional<std::string> alias;
};

struct Ap final : public Expr {
  Ap() = default;
  Ap(Pointer<Expr> f, Pointer<Expr> a) : func(f), arg(a) {}
  static Pointer<Ap> Create(Pointer<Expr> f, Pointer<Expr> a);
  std::string ToString() const override;
  bool isAp() const override { return true; }

  Pointer<Expr> func;
  Pointer<Expr> arg;
};

struct Number final : public Expr {
  Number() = default;
  Number(int64_t v) : value(v) {}
  std::string ToString() const override { return std::to_string(value); }
  bool isNumber() const override { return true; }
  int64_t value;
};
