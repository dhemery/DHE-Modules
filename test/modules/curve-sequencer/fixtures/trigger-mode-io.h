#pragma once

#include "modules/curve-sequencer/trigger-mode.h"
#include <ostream>

namespace dhe {
namespace curve_sequencer {

static auto name_of(dhe::curve_sequencer::TriggerMode mode) -> char const * {
  switch (mode) {
  case TriggerMode::GateRises:
    return "TriggerMode::GateRises";
  case TriggerMode::GateFalls:
    return "TriggerMode::GateFalls";
  case TriggerMode::GateChanges:
    return "TriggerMode::GateChanges";
  case TriggerMode::GateIsHigh:
    return "TriggerMode::GateIsHigh";
  case TriggerMode::GateIsLow:
    return "TriggerMode::GateIsLow";
  }
}

static inline auto operator<<(std::ostream &os, TriggerMode mode)
    -> std::ostream & {
  os << name_of(mode);
  return os;
}

} // namespace curve_sequencer
} // namespace dhe
