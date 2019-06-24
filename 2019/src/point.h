#pragma once

struct Point {
  int x;
  int y;
};

template<typename T>
T parse(const char*&);
