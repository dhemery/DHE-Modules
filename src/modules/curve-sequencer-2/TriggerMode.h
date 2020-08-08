#pragma once

namespace dhe {
namespace curve_sequencer_2 {
  enum class TriggerMode { GateRises, GateFalls, GateChanges, GateIsHigh, GateIsLow };

  static auto constexpr triggerModeCount = static_cast<int>(TriggerMode::GateIsLow) + 1;
} // namespace curve_sequencer_2
} // namespace dhe
