#pragma once

#include <memory>
#include <optional>
#include <string>
#include <unordered_map>

#include "expr.h"

class Galaxy {
 public:
  Galaxy();
  Galaxy(const std::string& filepath);

  Pointer<Expr> process(const std::string& message);

 private:
  void initBuiltins();

  Pointer<Expr> buildTree(std::istream& tokens);
  Pointer<Expr> eval(Pointer<Expr> expr);
  Pointer<Expr> tryEval(Pointer<Expr> expr);
  Pointer<Expr> evalCons(Pointer<Expr> y, Pointer<Expr> x);
  Pointer<Expr> refer(const std::string& name);
  Pointer<Atom> define(const std::string& name, Pointer<Expr> definition=nullptr);

  int64_t valueOf(Pointer<Expr> expr);

  // table[name] := Atom(name, alias) --(evaluated)--> Expr(definition)
  std::unordered_map<std::string, Pointer<Atom>> definition_table_;
};
