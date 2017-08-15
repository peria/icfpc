#pragma once

#include <vector>

#include "json.h"

// SiteId = Nat
// Site = { "id": SiteId }
struct Site {
  explicit Site(const Json& json);
  Json serialize() const;

  int id;
};

// River = { "source": SiteId, "target": SiteId }
struct River {
  River(const Json& json);
  Json serialize() const;

  int source;
  int target;
};

// Map = { "sites": [Site], "rivers": [River], "mines": [SiteId] }
struct Map {
  explicit Map(const Json& json);
  Json serialize() const;

  std::vector<Site> sites;
  std::vector<River> rivers;
  std::vector<int> mines;
};

// Move = {"claim": {"punter": PunterId, "source": SiteId, "target": SiteId}}
//      | {"pass": {"punter": PunterId}}
//      | {"splurge": {"punter": PunterId, "route": [SiteId]}}
//      | {"option": {"punter": PunterId, "source": SiteId, "target": SiteId}}
struct Move {
  enum class Type {
    Claim,
    Pass,
    Splurge,
    Option,
  };

  Move();
  explicit Move(const Json& json);
  void init(Type t, const Json& json);
  Json serialize() const;

  Type type;
  int punter;
  std::vector<int> site_ids;
};
