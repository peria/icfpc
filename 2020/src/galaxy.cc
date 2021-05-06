#include "galaxy.h"

#include <fstream>
#include <memory>
#include <sstream>
#include <stack>
#include <string>
#include <glog/logging.h>

namespace {

const char* kF38Impl = "ap ap ap if0 ap car x0 ( ap modem ap car ap cdr x0 , "
  "ap multipledraw ap car ap cdr ap cdr x0 ) ap ap ap interact x2 ap modem ap "
  "car ap cdr x0 ap send ap car ap cdr ap cdr x0";

}  // namespace

Galaxy::Galaxy() {
  initBuiltins();
}

Galaxy::Galaxy(const std::string& filepath) {
  initBuiltins();
  std::ifstream ifs(filepath);
  std::string line;
  while (std::getline(ifs, line)) {
    process(line);
  }
}

void Galaxy::initBuiltins() {
  // Built-in operations
  static const char* kBuiltinNames[] = {
      "neg", "i", "nil", "isnil", "car", "cdr", "t", "f", "add",
      "mul", "div", "lt",  "eq", "cons", "vec", "s", "c", "b",
      "f38", "interact",
      // Compiled operations
      "(", ",", ")",
  };
  for (auto name : kBuiltinNames) {
    define(name, Expr::Create(name));
  }
  process("checkerboard = ap ap s ap ap b s ap ap c ap ap b c ap ap b ap c ap "
          "c ap ap s ap ap b s ap ap b ap b ap ap s i i lt eq ap ap s mul i "
          "nil ap ap s ap ap b s ap ap b ap b cons ap ap s ap ap b s ap ap b "
          "ap b cons ap c div ap c ap ap s ap ap b b ap ap c ap ap b b add neg "
          "ap ap b ap s mul div ap ap c ap ap b b checkerboard ap ap c add 2");

  // Aliases
  static const char* kNameAliases[][2] = {
    {":1117", "pwr2"},
  };
  for (auto name_alias : kNameAliases) {
    define(name_alias[0]);
    definition_table_[name_alias[0]]->alias = name_alias[1];
  }
}

Pointer<Expr> Galaxy::process(const std::string& line) {
  std::istringstream tokens(line);
  if (line.find('=') != std::string::npos) {
    std::string identifier, equal;
    tokens >> identifier >> equal;
    // TODO: Work for "ap ap name x0 x1 = ....".
    CHECK_EQ(equal, "=");
    return define(identifier, buildTree(tokens));
  } else {
    return eval(buildTree(tokens));
  }
}

std::string Galaxy::click(int64 x, int64 y) {
  return "{}";
}

Pointer<Expr> Galaxy::buildTree(std::istream& tokens, LocalVarsMap&& local_vars) {
  // Pseudo root. Actual root is root->func.
  auto root = std::make_shared<Ap>();

  std::string token;
  std::stack<Pointer<Expr>> stack;
  stack.push(root);
  while (tokens >> token) {
    CHECK_NE(token, "=");
    Pointer<Expr> expr;
    // TODO: Remove the hack for list expressions.
    if (local_vars.count(token)) {
      expr = local_vars[token];
    } else if (token == "(" || token == ",") {
      expr = Ap::Create(Ap::Create(refer("cons"), nullptr), nullptr);
    } else if (token == ")") {
      expr = refer("nil");
    } else {
      expr = Expr::Create(token);
    }

    if (auto ap = As<Ap>(stack.top())) {
      if (!ap->func) {
        ap->func = expr;
      } else if (!ap->arg) {
        ap->arg = expr;
        stack.pop();
      } else {
        CHECK(false) << "Top AP is fullfilled.";
      }
    }
    if (expr->isAp()) {
      stack.push(expr);
    }
  }
  CHECK(!root->arg);
  root->func = compile(root->func);
  return root->func;
}

Pointer<Expr> Galaxy::compile(Pointer<Expr> expr) {
  return expr;
}

Pointer<Expr> Galaxy::eval(Pointer<Expr> expr) {
  if (expr->evaluated)
    return expr->evaluated;

  Pointer<Expr> initial_expr = expr;
  while (true) {
    Pointer<Expr> result = tryEval(expr);
    if (result == expr) {
      initial_expr->evaluated = result;
      return result;
    }
    expr = result;
  }
}

Pointer<Expr> Galaxy::tryEval(Pointer<Expr> expr) {
  if (expr->evaluated)
    return expr->evaluated;

  if (expr->isAtom())
    return refer(As<Atom>(expr)->name);

  if (!expr->isAp())
    return expr;
  auto ap = As<Ap>(expr);
  if (!ap->func || !ap->arg)
    return expr;

  auto func = eval(ap->func);
  auto x = ap->arg;
  if (func->isAtom()) {
    auto atom = As<Atom>(func);
    if (atom->name == "neg")
      return Expr::Create(-valueOf(x));
    if (atom->name == "i")
      return x;
    if (atom->name == "nil")
      return refer("t");
    if (atom->name == "isnil")
      return Ap::Create(
          x, Ap::Create(refer("t"), Ap::Create(refer("t"), refer("f"))));
    if (atom->name == "car")
      return Ap::Create(x, refer("t"));
    if (atom->name == "cdr")
      return Ap::Create(x, refer("f"));
    if (atom->name == "modem")
      return Ap::Create(refer("dem"), Ap::Create(refer("mod"), x));
  }

  if (!func->isAp())
    return expr;

  auto ap2 = As<Ap>(func);
  if (!ap2->func || !ap2->arg)
    return expr;

  auto func2 = eval(ap2->func);
  auto y = ap2->arg;
  if (func2->isAtom()) {
    auto atom = As<Atom>(func2);
    if (atom->name == "t")
      return y;
    if (atom->name == "f")
      return x;
    if (atom->name == "add")
      return Expr::Create(valueOf(y) + valueOf(x));
    if (atom->name == "mul")
      return Expr::Create(valueOf(y) * valueOf(x));
    if (atom->name == "div")
      return Expr::Create(valueOf(y) / valueOf(x));
    if (atom->name == "lt")
      return (valueOf(y) < valueOf(x)) ? refer("t") : refer("f");
    if (atom->name == "eq")
      return (valueOf(y) == valueOf(x)) ? refer("t") : refer("f");
    if (atom->name == "cons" || atom->name == "vec")
      return evalCons(y, x);
    if (atom->name == "f38") {
      std::istringstream f38s(kF38Impl);
      return buildTree(f38s, {{"x2", y}, {"x0", x}});
    }
  }
  if (!func2->isAp())
    return expr;

  auto ap3 = As<Ap>(func2);
  if (!ap3->func || !ap3->arg)
    return expr;

  auto func3 = eval(ap3->func);
  auto z = ap3->arg;
  if (func3->isAtom()) {
    auto atom = As<Atom>(func3);
    if (atom->name == "s")
      return Ap::Create(Ap::Create(z, x), Ap::Create(y, x));
    if (atom->name == "c")
      return Ap::Create(Ap::Create(z, x), y);
    if (atom->name == "b")
      return Ap::Create(z, Ap::Create(y, x));
    if (atom->name == "cons")
      return Ap::Create(Ap::Create(x, z), y);
    if (atom->name == "interact")
      return Ap::Create(Ap::Create(refer("f38"), z), Ap::Create(Ap::Create(z, y), x));
  }

  return expr;
}

Pointer<Expr> Galaxy::evalCons(Pointer<Expr> a, Pointer<Expr> b) {
  auto res = Ap::Create(Ap::Create(refer("cons"), eval(a)), eval(b));
  res->evaluated = res;
  return res;
}

Pointer<Expr> Galaxy::refer(const std::string& name) {
  if (!definition_table_.count(name)) {
    auto atom = std::make_shared<Atom>(name);
    definition_table_[name] = atom;
  }
  auto symbol = definition_table_[name];
  return symbol->evaluated ? symbol->evaluated : symbol;
}

Pointer<Atom> Galaxy::define(const std::string& name,
                             Pointer<Expr> definition) {
  if (!definition_table_.count(name))
    definition_table_[name] = std::make_shared<Atom>(name);

  auto symbol = definition_table_[name];
  CHECK(!symbol->evaluated);
  symbol->evaluated = definition;
  if (symbol->hasAlias())
    define(symbol->alias.value(), definition);

  return definition_table_[name];
}

int64_t Galaxy::valueOf(Pointer<Expr> expr) {
  return As<Number>(eval(expr))->value;
}
