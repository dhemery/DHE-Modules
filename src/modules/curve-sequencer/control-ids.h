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
    ONE_PER_STEP(StepCurvature, N),
    ONE_PER_STEP(StepDuration, N),
    ONE_PER_STEP(StepEnabled, N),
    ONE_PER_STEP(StepLevel, N),
    ONE_PER_STEP(StepGenerateMode, N),
    ONE_PER_STEP(StepAdvanceMode, N),
    ONE_PER_STEP(StepShape, N),
    GenerateModeMenu,
    Count
  };
};

template <int N> struct InputIds {
  enum { Main, Gate, Loop, Reset, Run, ONE_PER_STEP(StepEnabled, N), Count };
};

struct OutputIds {
  enum { Main, Count };
};

template <int N> struct LightIds {
  enum { TWO_PER_STEP(StepProgress, N), Count };
};

} // namespace curve_sequencer
} // namespace dhe

#undef PER_STEP
#undef ONE_PER_STEP
#undef TWO_PER_STEP
