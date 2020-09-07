#pragma once

#include "controls.h"

namespace dhe {
namespace curve_sequencer {

namespace json_v0 {

// Params whose meanings changed from json v0 to v1. We must translate the
// values of these params whenever we load a v0 patch.
template <int N> struct ParamIds {
  enum {
    StepGenerateMode = ParamIds<N>::StepTriggerMode,  // Values: GenerateMode
    StepAdvanceMode = ParamIds<N>::StepInterruptMode, // Values: AdvanceMode
  };
};

enum class GenerateMode {
  Curve,   // Start: Sample OUT; End: Track LEVEL
  Hold,    // Start: Sample OUT; End: Sample Out; Int: Ignore; At End: Advance
  Sustain, // Start: Sample OUT; End: Sample Out; Int: Advance; At End: Sustain
  Input,   // Start: Track IN; End: Track IN
  Chase,   // Start: Sample OUT; End: Track IN
  Level,   // Start: Track LEVEL; End: Track LEVEL
};

enum class AdvanceMode {
  // Unused after v0. Translate this value to:
  // - Int: Ignore
  // - At End: Advance
  // We can leave the param value itself at 0. In v1 this translates to
  // GateRises, which is a reasonable default. And besides, the choice of
  // trigger mode has no effect on patch behavior, because the Int and At End
  // settings leave the step ignoring all triggers.
  TimerExpires,
  // To update the rest of the values:
  // - Trigger: v0::AdvanceMode::X -> v1::TriggerMode::X
  // - Int: Advance
  // - At End: Advance
  GateRises,
  GateFalls,
  GateChanges,
  GateIsHigh,
  GateIsLow
}

} // namespace json_v0
} // namespace curve_sequencer
} // namespace dhe
