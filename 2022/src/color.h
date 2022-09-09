#pragma once

#include <array>
#include <cmath>
#include <cstdint>

// Named "RGBA" in the spec.
struct Color {
  Color() { color.u32 = 0xffffffffu; }
  Color(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a) {
    color.rgba.r = r;
    color.rgba.g = g;
    color.rgba.b = b;
    color.rgba.a = a;
  }

  union {
    std::array<std::uint8_t, 4> array;
    struct {
      std::uint8_t r;
      std::uint8_t g;
      std::uint8_t b;
      std::uint8_t a;
    } rgba;
    std::uint32_t u32;
  } color;

  std::uint8_t& r() { return color.rgba.r; }
  std::uint8_t& g() { return color.rgba.g; }
  std::uint8_t& b() { return color.rgba.b; }
  std::uint8_t& a() { return color.rgba.a; }
  std::uint8_t& operator[](std::size_t i) { return color.array[i]; }

  std::uint8_t r() const { return color.rgba.r; }
  std::uint8_t g() const { return color.rgba.g; }
  std::uint8_t b() const { return color.rgba.b; }
  std::uint8_t a() const { return color.rgba.a; }
  std::uint8_t operator[](std::size_t i) const { return color.array[i]; }

  bool operator<(const Color& rhs) const {
    const Color& lhs = *this;
    for (int i = 0; i < 3; ++i) {
      if (lhs[i] != rhs[i]) {
        return lhs[i] < rhs[i];
      }
    }
    return lhs[3] < rhs[3];
  }

  static inline double distance(const Color& lhs, const Color& rhs) {
    int dr = lhs.r() - rhs.r();
    int dg = lhs.g() - rhs.g();
    int db = lhs.b() - rhs.b();
    int da = lhs.a() - rhs.a();
    return std::sqrt(dr * dr + dg * dg + db * db + da * da);
  }
};
