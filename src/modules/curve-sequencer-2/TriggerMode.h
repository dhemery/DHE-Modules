#pragma once

namespace dhe {
namespace curve_sequencer {
  enum class TriggerMode { GateRises, GateFalls, GateChanges, GateIsHigh, GateIsLow };

  static auto constexpr triggerModeCount = static_cast<int>(TriggerMode::GateIsLow) + 1;
} // namespace curve_sequencer
} // namespace dhe
