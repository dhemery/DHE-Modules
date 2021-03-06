#pragma once

namespace dhe {
namespace curve_sequencer {
enum class AdvanceMode {
  TimerExpires,
  GateRises,
  GateFalls,
  GateChanges,
  GateIsHigh,
  GateIsLow
};

static auto constexpr advance_mode_count =
    static_cast<int>(AdvanceMode::GateIsLow) + 1;
} // namespace curve_sequencer
} // namespace dhe
