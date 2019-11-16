#pragma once

namespace dhe {
namespace curve_sequencer {
  enum class StepCondition { TimerExpires, GateIsLow, GateIsHigh, GateRises, GateFalls, GateChanges };
} // namespace curve_sequencer
} // namespace dhe
