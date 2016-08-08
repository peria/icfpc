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
