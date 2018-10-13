#include "trace.h"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

std::vector<uint8> Trace::toString() const {
  std::vector<uint8> buf;
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
        const LLD& lld = command->To<SMove>()->lld;
        buf.push_back(0b00000100 + (lld.a() << 4));
        buf.push_back(lld.i());
        break;
      }
      case Command::kLMove: {
        const SLD& sld1 = command->To<LMove>()->sld1;
        const SLD& sld2 = command->To<LMove>()->sld2;
        buf.push_back((sld2.a() << 6) + (sld1.a() << 4) + 0b1100);
        buf.push_back((sld2.i() << 4) + sld1.i());
        break;
      }
      case Command::kFission: {
        const ND& nd = command->To<Fission>()->nd;
        int m = command->To<Fission>()->m;
        buf.push_back((nd.nd() << 3) + 0b101);
        buf.push_back(m);
        break;
      }
      case Command::kFill: {
        const ND& nd = command->To<Fill>()->nd;
        buf.push_back((nd.nd() << 3) + 0b011);
        break;
      }
      case Command::kVoid: {
        const ND& nd = command->To<Void>()->nd;
        buf.push_back((nd.nd() << 3) + 0b010);
        break;
      }
      case Command::kFusionP: {
        const ND& nd = command->To<FusionP>()->nd;
        buf.push_back((nd.nd() << 3) + 0b111);
        break;
      }
      case Command::kFusionS: {
        const ND& nd = command->To<FusionS>()->nd;
        buf.push_back((nd.nd() << 3) + 0b110);
        break;
      }
      case Command::kGFill: {
        const ND& nd = command->To<GFill>()->nd;
        const FD& fd = command->To<GFill>()->fd;
        buf.push_back((nd.nd() << 3) + 0b001);
        buf.push_back(fd.x + 30);
        buf.push_back(fd.y + 30);
        buf.push_back(fd.z + 30);
        break;
      }
      case Command::kGVoid: {
        const ND& nd = command->To<GVoid>()->nd;
        const FD& fd = command->To<GVoid>()->fd;
        buf.push_back((nd.nd() << 3) + 0b000);
        buf.push_back(fd.x + 30);
        buf.push_back(fd.y + 30);
        buf.push_back(fd.z + 30);
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
  ofs.write(reinterpret_cast<const char*>(buf.data()), buf.size());
  ofs.close();
}
