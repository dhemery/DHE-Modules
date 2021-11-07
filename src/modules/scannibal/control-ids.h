#pragma once

#define PER_STEP(name, n) name, name##_LAST = (name) + (n)-1
#define ONE_PER_STEP(name, n) PER_STEP(name, n)
#define TWO_PER_STEP(name, n) PER_STEP(name, (n)*2)

namespace dhe {
namespace scannibal {

template <int N> struct ParamIds {
  enum {
    SelectionLength,
    LevelRange,
    ONE_PER_STEP(StepCurvature, N),
    ONE_PER_STEP(StepDuration, N),
    ONE_PER_STEP(StepPhase0AnchorLevel, N),
    ONE_PER_STEP(StepPhase0AnchorMode, N),
    ONE_PER_STEP(StepPhase0AnchorSource, N),
    ONE_PER_STEP(StepPhase1AnchorLevel, N),
    ONE_PER_STEP(StepPhase1AnchorMode, N),
    ONE_PER_STEP(StepPhase1AnchorSource, N),
    ONE_PER_STEP(StepShape, N),
    Count,
  };
};

template <int N> struct InputIds {
  enum {
    InA,
    InB,
    InC,
    Phase,
    ONE_PER_STEP(StepCurvatureCv, N),
    ONE_PER_STEP(StepDurationCv, N),
    ONE_PER_STEP(StepPhase0AnchorLevelCv, N),
    ONE_PER_STEP(StepPhase1AnchorLevelCv, N),
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

} // namespace scannibal
} // namespace dhe

#undef PER_STEP
#undef ONE_PER_STEP
#undef TWO_PER_STEP
