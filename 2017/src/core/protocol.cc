#include "protocol.h"

#include <glog/logging.h>

#include "json.h"

Site::Site(const Json& json) {
  DCHECK(json.find("id") != json.end());
  id = json["id"].get<int>();
}

Json Site::serialize() const {
  Json json = {"id", id};
  return json;
}

River::River(const Json& json) {
  DCHECK(json.find("source") != json.end());
  DCHECK(json.find("target") != json.end());
  source = json["source"];
  target = json["target"];
}

Json River::serialize() const {
  Json json;
  json["source"] = source;
  json["target"] = target;
  return json;
}

Map::Map(const Json& json) {
  CHECK(json.find("sites") != json.end());
  for (auto& jsite : json["sites"]) {
    sites.emplace_back(jsite);
  }
  CHECK(json.find("rivers") != json.end());
  for (auto& jriver : json["rivers"]) {
    rivers.emplace_back(jriver);
  }
  CHECK(json.find("mines") != json.end());
  for (int site_id : json["mines"]) {
    mines.push_back(site_id);
  }
}

Json Map::serialize() const {
  Json json;
  for (auto& site : sites) {
    json["sites"].push_back(site.serialize());
  }
  for (auto& river : rivers) {
    json["rivers"].push_back(river.serialize());
  }
  json["mines"] = mines;
  return json;
}

Move::Move(const Json& json) {
  if (json.find("pass") != json.end()) {
    init(Type::Pass, json["pass"]);
  } else if (json.find("claim") != json.end()) {
    init(Type::Claim, json["claim"]);
  } else if (json.find("splurge") != json.end()) {
    init(Type::Splurge, json["splurge"]);
  } else if (json.find("option") != json.end()) {
    init(Type::Option, json["option"]);
  } else {
    // Not reached
    DCHECK(false);
  }
}

void Move::init(Type t, const Json& json) {
  type = t;
  DCHECK(json.find("punter") != json.end());
  punter = json["punter"];
  switch (type) {
  case Type::Claim:
  case Type::Option: {
    site_ids.resize(2);
    DCHECK(json.find("source") != json.end());
    DCHECK(json.find("target") != json.end());
    site_ids[0] = json["source"];
    site_ids[1] = json["target"];
    break;
  }
  case Type::Splurge: {
    DCHECK(json.find("route") != json.end());
    for (auto& j : json["route"]) {
      site_ids.push_back(j);
    }
    break;
  }
  case Type::Pass: {
    break;
  }
  }
}

Json Move::serialize() const {
  switch (type) {
  case Type::Claim: {
    return Json {{
      "claim", {
        {"punter", punter},
        {"source", site_ids[0]},
        {"target", site_ids[1]}
      }
    }};
  }
  case Type::Pass:
    return Json {{
      "pass", {
        {"punter", punter}
      }
    }};
  case Type::Splurge: {
    Json json = site_ids;
    return Json {{"splurge", {{"punter", punter}, {"route", json}} }};
  }
  case Type::Option: {
    return Json {{
      "option", {
        {"punter", punter},
        {"source", site_ids[0]},
        {"target", site_ids[1]}
      }
    }};
  }
  }
  CHECK(false);
  return Json();
}
