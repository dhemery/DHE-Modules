#pragma once

#include "advance-mode.h"
#include "generate-mode.h"

#include "signals/common-inputs.h"
#include "signals/curvature-inputs.h"
#include "signals/duration-inputs.h"
#include "signals/level-inputs.h"

#include <vector>

#define PER_STEP(name, n) name, name##_LAST = (name) + (n)-1
#define ONE_PER_STEP(name, n) PER_STEP(name, n)
#define TWO_PER_STEP(name, n) PER_STEP(name, (n)*2)

namespace dhe {
namespace curve_sequencer {

template <int N> struct ParamIds {
  enum {
    Run,
    Gate,
    SelectionStart,
    SelectionLength,
    Loop,
    Reset,
    DurationRange,
    LevelRange,
    ONE_PER_STEP(Curvature, N),
    ONE_PER_STEP(Duration, N),
    ONE_PER_STEP(Enabled, N),
    ONE_PER_STEP(Level, N),
    ONE_PER_STEP(GenerateMode, N),
    ONE_PER_STEP(AdvanceMode, N),
    ONE_PER_STEP(Shape, N),
    GenerateModeMenu,
    Count
  };
};

template <int N> struct InputIds {
  enum {
    CurveSequencerInput,
    GateInput,
    LoopInput,
    ResetInput,
    RunInput,
    ONE_PER_STEP(EnabledInputs, N),
    InputCount
  };
};

struct OutputIds {
  enum { CurveSequencerOutput, OutputCount };
};

template <int N> struct LightIds {
  enum { TWO_PER_STEP(ProgressLights, N), LightCount };
};

} // namespace curve_sequencer
} // namespace dhe

#undef PER_STEP
#undef ONE_PER_STEP
#undef TWO_PER_STEP
