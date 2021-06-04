#pragma once

#include "modules/curve-sequencer/advance-mode.h"

#include <array>
#include <ostream>
#include <string>

namespace test {
namespace curve_sequencer {
using dhe::curve_sequencer::advance_mode_count;
using dhe::curve_sequencer::AdvanceMode;

static auto constexpr advance_modes =
    std::array<AdvanceMode, advance_mode_count>{
        AdvanceMode::TimerExpires, AdvanceMode::GateRises,
        AdvanceMode::GateFalls,    AdvanceMode::GateChanges,
        AdvanceMode::GateIsHigh,   AdvanceMode::GateIsLow,
    };

static inline auto name_of(AdvanceMode mode) -> std::string {
  switch (mode) {
  case AdvanceMode::TimerExpires:
    return "AdvanceMode::TimerExpires";
  case AdvanceMode::GateRises:
    return "AdvanceMode::GateRises";
  case AdvanceMode::GateFalls:
    return "AdvanceMode::GateFalls";
  case AdvanceMode::GateChanges:
    return "AdvanceMode::GateChanges";
  case AdvanceMode::GateIsHigh:
    return "AdvanceMode::GateIsHigh";
  case AdvanceMode::GateIsLow:
    return "AdvanceMode::GateIsLow";
  default:
    return "Unknown AdvanceMode " + std::to_string(static_cast<int>(mode));
  }
}
} // namespace curve_sequencer
} // namespace test

namespace dhe {
namespace curve_sequencer {
static inline auto operator<<(std::ostream &os, AdvanceMode mode)
    -> std::ostream & {
  os << test::curve_sequencer::name_of(mode);
  return os;
}
} // namespace curve_sequencer
} // namespace dhe
