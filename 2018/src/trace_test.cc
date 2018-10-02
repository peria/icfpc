#include "trace.h"

#include <gtest/gtest.h>
#include <string>
#include <vector>

#include "command.h"

TEST(TraceTest, SMove) {
  Trace trace;
  trace.emplace_back(new SMove(LLD(12, 0, 0)));
  trace.emplace_back(new SMove(LLD(0, 0, -4)));

  auto dump = trace.toString();
  EXPECT_EQ(4u, dump.size());
  EXPECT_EQ(0b00010100, dump[0]);
  EXPECT_EQ(0b00011011, dump[1]);
  EXPECT_EQ(0b00110100, dump[2]);
  EXPECT_EQ(0b00001011, dump[3]);
}

TEST(TraceTest, LMove) {
  Trace trace;
  trace.emplace_back(new LMove(SLD(3, 0, 0), SLD(0, -5, 0)));
  trace.emplace_back(new LMove(SLD(0, -2, 0), SLD(0, 0, 2)));

  auto dump = trace.toString();
  EXPECT_EQ(4u, dump.size());
  EXPECT_EQ(0b10011100, dump[0]);
  EXPECT_EQ(0b00001000, dump[1]);
  EXPECT_EQ(0b11101100, dump[2]);
  EXPECT_EQ(0b01110011, dump[3]);
}

TEST(TraceTest, Fusion) {
  Trace trace;
  trace.emplace_back(new FusionP(ND(-1, 1, 0)));
  trace.emplace_back(new FusionS(ND(1, -1, 0)));

  auto dump = trace.toString();
  EXPECT_EQ(2u, dump.size());
  EXPECT_EQ(0b00111111, dump[0]);
  EXPECT_EQ(0b10011110, dump[1]);
}

TEST(TraceTest, Fission) {
  Trace trace;
  trace.emplace_back(new Fission(ND(0, 0, 1), 5));

  auto dump = trace.toString();
  EXPECT_EQ(2u, dump.size());
  EXPECT_EQ(0b01110101, dump[0]);
  EXPECT_EQ(0b00000101, dump[1]);
}

TEST(TraceTest, FillVoid) {
  Trace trace;
  trace.emplace_back(new Fill(ND(0, -1, 0)));
  trace.emplace_back(new Void(ND(1, 0, 1)));

  auto dump = trace.toString();
  EXPECT_EQ(2u, dump.size());
  EXPECT_EQ(0b01010011, dump[0]);
  EXPECT_EQ(0b10111010, dump[1]);
}

TEST(TraceTest, GFill) {
  Trace trace;
  trace.emplace_back(new GFill(ND(0, -1, 0), FD(10, -15, 20)));

  auto dump = trace.toString();
  EXPECT_EQ(4u, dump.size());
  EXPECT_EQ(0b01010001, dump[0]);
  EXPECT_EQ(0b00101000, dump[1]);
  EXPECT_EQ(0b00001111, dump[2]);
  EXPECT_EQ(0b00110010, dump[3]);
}

TEST(TraceTest, GVoid) {
  Trace trace;
  trace.emplace_back(new GVoid(ND(1, 0, 0), FD(5, 5, -5)));

  auto dump = trace.toString();
  EXPECT_EQ(4u, dump.size());
  EXPECT_EQ(0b10110000, dump[0]);
  EXPECT_EQ(0b00100011, dump[1]);
  EXPECT_EQ(0b00100011, dump[2]);
  EXPECT_EQ(0b00011001, dump[3]);
}
