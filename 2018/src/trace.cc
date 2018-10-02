#include "trace.h"

#include <fstream>
#include <iostream>
#include <string>

std::string Trace::toString() const {
  std::string buf;
  for (auto& command : (*this)) {
    switch (command->type()) {
      case Command::kHalt:
        buf.push_back(0b11111111);
        break;
      case Command::kWait:
        buf.push_back(0b11111110);
        break;
      case Command::kFlip:
        buf.push_back(0b11111101);
        break;
      case Command::kSMove: {
        const Coordinate& lld = command->toSMove()->lld;
        buf.push_back(0b00000100 + lld.a());
        buf.push_back(lld.i());
        break;
      }
      case Command::kLMove: {
        const Coordinate& sld1 = command->toLMove()->sld1;
        const Coordinate& sld2 = command->toLMove()->sld2;
        buf.push_back((sld2.a() << 6) + (sld1.a() << 4) + 0b1100);
        buf.push_back((sld2.i() << 4) + sld1.i());
        break;
      }
      case Command::kFission: {
        const Coordinate& nd = command->toFission()->nd;
        int m = command->toFission()->m;
        buf.push_back((nd.nd() << 3) + 0b101);
        buf.push_back(m);
        break;
      }
      case Command::kFill: {
        const Coordinate& nd = command->toFill()->nd;
        buf.push_back((nd.nd() << 3) + 0b011);
        break;
      }
      case Command::kVoid: {
        const Coordinate& nd = command->toVoid()->nd;
        buf.push_back((nd.nd() << 3) + 0b010);
        break;
      }
      case Command::kFusionP: {
        const Coordinate& nd = command->toFusionP()->nd;
        buf.push_back((nd.nd() << 3) + 0b111);
        break;
      }
      case Command::kFusionS: {
        const Coordinate& nd = command->toFusionS()->nd;
        buf.push_back((nd.nd() << 3) + 0b110);
        break;
      }
      case Command::kGFill: {
        const Coordinate& nd = command->toGFill()->nd;
        const Coordinate& fd = command->toGFill()->fd;
        buf.push_back((nd.nd() << 3) + 0b001);
        buf.push_back(fd.x);
        buf.push_back(fd.y);
        buf.push_back(fd.z);
        break;
      }
      case Command::kGVoid: {
        const Coordinate& nd = command->toGVoid()->nd;
        const Coordinate& fd = command->toGVoid()->fd;
        buf.push_back((nd.nd() << 3) + 0b000);
        buf.push_back(fd.x);
        buf.push_back(fd.y);
        buf.push_back(fd.z);
        break;
      }
      case Command::kSync: {
        break;
      }
    }
  }
  return buf;
}

void Trace::dump(const std::string& filename) const {
  std::ofstream ofs(filename, std::ofstream::binary);
  if (!ofs) {
    std::cerr << "Fail to output to " << filename << "\n";
    return;
  }
  auto buf = toString();
  ofs.write(buf.data(), buf.size());
  ofs.close();
}
