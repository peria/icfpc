#include "command.h"

std::ostream& operator<<(std::ostream& ost, const Command& cmd) {
  switch (cmd.type()) {
    case Command::kHalt:
      ost << "<Halt>";
      break;
    case Command::kWait:
      ost << "<Wait>";
      break;
    case Command::kFlip:
      ost << "<Flip>";
      break;
    case Command::kSMove:
      ost << "<SMove>";
      break;
    case Command::kLMove:
      ost << "<LMove>";
      break;
    case Command::kFission:
      ost << "<Fission>";
      break;
    case Command::kFill:
      ost << "<Fill>";
      break;
    case Command::kVoid:
      ost << "<Void>";
      break;
    case Command::kFusionP:
      ost << "<FusionP>";
      break;
    case Command::kFusionS:
      ost << "<FusionS>";
      break;
    case Command::kGFill:
      ost << "<GFill>";
      break;
    case Command::kGVoid:
      ost << "<GVoid>";
      break;
    case Command::kSync:
      ost << "<*Sync*>";
      break;
  }
  return ost;
}
