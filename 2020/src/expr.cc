#include "expr.h"

#include <cctype>
#include <sstream>

Pointer<Expr> Expr::Create(const std::string& token) {
  if (token == "ap")
    return std::make_shared<Ap>();
  if (std::isdigit(token[0]) || token[0] == '-')
    return std::make_shared<Number>(std::stoll(token));
  return std::make_shared<Atom>(token);
}

Pointer<Expr> Expr::Create(int64_t number) {
  return std::make_shared<Number>(number);
}

std::string Expr::ToString() const {
  if (evaluated)
    return evaluated->ToString();
  return "";
}

Pointer<Ap> Ap::Create(Pointer<Expr> f, Pointer<Expr> a) {
  return std::make_shared<Ap>(f, a);
}

std::string Ap::ToString() const {
  std::ostringstream oss;
  oss << "ap";
  if (func)
    oss << " " << func->ToString();
  if (arg)
    oss << " " << arg->ToString();
  return oss.str();
}
