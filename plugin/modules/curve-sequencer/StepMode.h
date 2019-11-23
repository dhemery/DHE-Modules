#pragma once

namespace dhe {
namespace curve_sequencer {
  enum class StepMode { Curve, Hold, Sustain };

  static auto constexpr modeCount = static_cast<int>(StepMode::Sustain) + 1;
} // namespace curve_sequencer
} // namespace dhe
