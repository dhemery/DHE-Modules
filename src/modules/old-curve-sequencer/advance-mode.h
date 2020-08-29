#pragma once

namespace dhe {
namespace old_curve_sequencer {
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
} // namespace old_curve_sequencer
} // namespace dhe
