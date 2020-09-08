#pragma once

#include "controls.h"
#include "anchor.h"

#include <engine/Module.hpp>

namespace dhe {
namespace curve_sequencer {

namespace from_patch_v0 {

// Params whose meanings changed from json v0 to v1. We must translate the
// values of these params whenever we load a v0 patch.
template <int N> struct PatchParamIds {
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
};

template <int N> void update(rack::engine::Module *m) {
  INFO("from_patch_v0::update(m)");
  using PatchParam = PatchParamIds<N>;
  using ModuleParam = ParamIds<N>;

  INFO("HEY HEY IN UPDATE!");
  for (auto step = 0; step < N; step++) {
    m->params[ModuleParam::StepStartAnchorSource + step].setValue(
        static_cast<int>(AnchorSource::Aux));
    m->params[ModuleParam::StepStartLevel + step].setValue(
        m->params[ModuleParam::StepEndLevel + step].getValue());
  }
}
} // namespace from_patch_v0
} // namespace curve_sequencer
} // namespace dhe
