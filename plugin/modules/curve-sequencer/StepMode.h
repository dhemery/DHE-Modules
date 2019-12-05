#pragma once

namespace dhe {
namespace curve_sequencer {
  enum class StepMode { Curve, Hold, Sustain, Input, Chase, Level };

  static auto constexpr modeCount = static_cast<int>(StepMode::Level) + 1;
} // namespace curve_sequencer
} // namespace dhe
