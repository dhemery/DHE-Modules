#pragma once

#define PER_STEP(name, n) name, name##_LAST = (name) + (n)-1
#define ONE_PER_STEP(name, n) PER_STEP(name, n)
#define TWO_PER_STEP(name, n) PER_STEP(name, (n)*2)

namespace dhe {
namespace sequencizer {

template <int N> struct ParamIds {
  enum {
    DurationMultiplier,
    DurationRange,
    Gate,
    LevelMultiplier,
    LevelRange,
    Loop,
    Reset,
    Run,
    SelectionLength,
    SelectionStart,
    ONE_PER_STEP(StepCurvature, N),
    ONE_PER_STEP(StepDuration, N),
    ONE_PER_STEP(StepEnabled, N),
    ONE_PER_STEP(StepEndAnchorLevel, N),
    ONE_PER_STEP(StepEndAnchorMode, N),
    ONE_PER_STEP(StepEndAnchorSource, N),
    ONE_PER_STEP(StepInterruptMode, N),
    ONE_PER_STEP(StepShape, N),
    ONE_PER_STEP(StepStartAnchorLevel, N),
    ONE_PER_STEP(StepStartAnchorMode, N),
    ONE_PER_STEP(StepStartAnchorSource, N),
    ONE_PER_STEP(StepSustainMode, N),
    ONE_PER_STEP(StepTriggerMode, N),
    Count,
  };
};

template <int N> struct InputIds {
  enum {
    DurationMultiplierCV,
    Gate,
    InA,
    InB,
    InC,
    LevelAttenuationCV,
    Loop,
    Reset,
    Run,
    Count,
  };
};

struct OutputIds {
  enum {
    IsCurving,
    IsSustaining,
    Out,
    SequenceEventPulse,
    StepEventPulse,
    StepNumber,
    Count,
  };
};

template <int N> struct LightIds {
  enum {
    TWO_PER_STEP(StepProgress, N),
    Count,
  };
};

} // namespace sequencizer
} // namespace dhe

#undef PER_STEP
#undef ONE_PER_STEP
#undef TWO_PER_STEP
