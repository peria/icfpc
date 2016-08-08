#include "map.h"

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
