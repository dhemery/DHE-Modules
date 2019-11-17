#pragma once

namespace dhe {
namespace curve_sequencer {
  enum class AdvanceCondition { TimerExpires, GateRises, GateFalls, GateChanges, GateIsHigh, GateIsLow };
} // namespace curve_sequencer
} // namespace dhe
