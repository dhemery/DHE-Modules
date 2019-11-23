#pragma once

namespace dhe {
namespace curve_sequencer {
  enum class AdvanceCondition { TimerExpires, GateRises, GateFalls, GateChanges, GateIsHigh, GateIsLow };

  static auto constexpr advanceConditionCount = static_cast<int>(AdvanceCondition::GateIsLow) + 1;
} // namespace curve_sequencer
} // namespace dhe
