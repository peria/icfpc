#pragma once

#include <glog/logging.h>

#include <cstdint>
#include <iostream>

using int64 = std::int64_t;
using int32 = std::int32_t;
using int16 = std::int16_t;
using int8 = std::int8_t;

using uint64 = std::uint64_t;
using uint32 = std::uint32_t;
using uint16 = std::uint16_t;
using uint8 = std::uint8_t;

#define NOT_IMPLEMENTED() CHECK(false) << "Not implemented."

int PopCnt(uint64);
