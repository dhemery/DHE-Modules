#pragma once

#define PER_STEP(name, n) name, name##_LAST = (name) + (n)-1
#define ONE_PER_STEP(name, n) PER_STEP(name, n)
#define TWO_PER_STEP(name, n) PER_STEP(name, (n)*2)

namespace dhe {
namespace scannibal {

template <int N> struct ParamIds {
  enum {
    Length,
    LevelRange,
    ONE_PER_STEP(Curvature, N),
    ONE_PER_STEP(Duration, N),
    ONE_PER_STEP(Phase0AnchorLevel, N),
    ONE_PER_STEP(Phase0AnchorMode, N),
    ONE_PER_STEP(Phase0AnchorSource, N),
    ONE_PER_STEP(Phase1AnchorLevel, N),
    ONE_PER_STEP(Phase1AnchorMode, N),
    ONE_PER_STEP(Phase1AnchorSource, N),
    ONE_PER_STEP(Shape, N),
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
    ONE_PER_STEP(Phase0AnchorLevelCV, N),
    ONE_PER_STEP(Phase1AnchorLevelCV, N),
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
