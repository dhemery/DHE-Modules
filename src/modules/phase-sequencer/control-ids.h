#pragma once

#define PER_STEP(name, n) name, name##_LAST = (name) + (n)-1
#define ONE_PER_STEP(name, n) PER_STEP(name, n)
#define TWO_PER_STEP(name, n) PER_STEP(name, (n)*2)

namespace dhe {
namespace phase_sequencer {

template <int N> struct ParamIds {
  enum {
    LevelRange,
    ONE_PER_STEP(Curvature, N),
    ONE_PER_STEP(Duration, N),
    ONE_PER_STEP(EndAnchorLevel, N),
    ONE_PER_STEP(EndAnchorMode, N),
    ONE_PER_STEP(EndAnchorSource, N),
    ONE_PER_STEP(Shape, N),
    ONE_PER_STEP(StartAnchorLevel, N),
    ONE_PER_STEP(StartAnchorMode, N),
    ONE_PER_STEP(StartAnchorSource, N),
    Count,
  };
};

template <int N> struct InputIds {
  enum {
    InA,
    InB,
    InC,
    Phase,
    ONE_PER_STEP(CurvatureCV, N),
    ONE_PER_STEP(DurationCV, N),
    ONE_PER_STEP(EndAnchorLevelCV, N),
    ONE_PER_STEP(StartAnchorLevelCV, N),
    Count,
  };
};

struct OutputIds {
  enum {
    Out,
    StepNumber,
    StepPhase,
    Count,
  };
};

template <int N> struct LightIds {
  enum {
    TWO_PER_STEP(Progress, N),
    Count,
  };
};

} // namespace phase_sequencer
} // namespace dhe

#undef PER_STEP
#undef ONE_PER_STEP
#undef TWO_PER_STEP
