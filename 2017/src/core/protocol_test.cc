#include "protocol.h"

#include <gtest/gtest.h>

TEST(ProtocolTest, MoveDeserialize) {
  Move claim(Json::parse(R"({"claim": {"punter": 5, "source": 3, "target": 2}})"));
  EXPECT_EQ(Move::Type::Claim, claim.type);
  EXPECT_EQ(5, claim.punter);
  EXPECT_EQ(2, static_cast<int>(claim.site_ids.size()));
  EXPECT_EQ(3, claim.site_ids[0]);
  EXPECT_EQ(2, claim.site_ids[1]);

  Move pass(Json::parse(R"({"pass": {"punter": 0}})"));
  EXPECT_EQ(Move::Type::Pass, pass.type);
  EXPECT_EQ(0, pass.punter);
  EXPECT_EQ(0, static_cast<int>(pass.site_ids.size()));

  Move splurge(Json::parse(R"({"splurge": {"punter": 1, "route": [3, 1, 4, 5, 9]}})"));
  EXPECT_EQ(Move::Type::Splurge, splurge.type);
  EXPECT_EQ(1, splurge.punter);
  EXPECT_EQ(5, static_cast<int>(splurge.site_ids.size()));
  EXPECT_EQ(3, splurge.site_ids[0]);
  EXPECT_EQ(1, splurge.site_ids[1]);
  EXPECT_EQ(4, splurge.site_ids[2]);
  EXPECT_EQ(5, splurge.site_ids[3]);
  EXPECT_EQ(9, splurge.site_ids[4]);

  Move option(Json::parse(R"({"option": {"punter": 9, "source": 8, "target": 7}})"));
  EXPECT_EQ(Move::Type::Option, option.type);
  EXPECT_EQ(9, option.punter);
  EXPECT_EQ(2, static_cast<int>(option.site_ids.size()));
  EXPECT_EQ(8, option.site_ids[0]);
  EXPECT_EQ(7, option.site_ids[1]);
}

TEST(ProtocolTest, MoveSerialize) {
  Move claim(Json::parse(R"({"claim": {"punter": 5, "source": 3, "target": 2}})"));
  EXPECT_EQ(R"({"claim":{"punter":5,"source":3,"target":2}})",
            claim.serialize().dump());
  Move pass(Json::parse(R"({"pass": {"punter": 0}})"));
  EXPECT_EQ(R"({"pass":{"punter":0}})",
            pass.serialize().dump());
  Move splurge(Json::parse(R"({"splurge": {"punter": 1, "route": [3, 1, 4, 5, 9]}})"));
  EXPECT_EQ(R"({"splurge":{"punter":1,"route":[3,1,4,5,9]}})",
            splurge.serialize().dump());
  Move option(Json::parse(R"({"option": {"punter": 9, "source": 8, "target": 7}})"));
  EXPECT_EQ(R"({"option":{"punter":9,"source":8,"target":7}})",
            option.serialize().dump());
}
