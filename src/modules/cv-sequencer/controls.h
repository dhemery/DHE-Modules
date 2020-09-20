#pragma once

#define PER_STEP(name, n) name, name##_LAST = (name) + (n)-1
#define ONE_PER_STEP(name, n) PER_STEP(name, n)
#define TWO_PER_STEP(name, n) PER_STEP(name, (n)*2)

namespace dhe {
namespace cv_sequencer {

template <int N> struct ParamIds {
  enum {
    Gate,
    GlobalDuration,
    GlobalDurationRange,
    GlobalLevel,
    GlobalLevelRange,
    Loop,
    Reset,
    Run,
    SelectionLength,
    SelectionStart,
    ONE_PER_STEP(StepCompletionMode, N),
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
    ONE_PER_STEP(StepTriggerMode, N),
    Count,
  };
};

template <int N> struct InputIds {
  enum {
    GlobalDurationCV,
    GlobalLevelCV,
    Gate,
    InA,
    InB,
    Loop,
    Reset,
    Run,
    Trigger,
    Count,
  };
};

struct OutputIds {
  enum {
    IsCurving,
    IsSustaining,
    Out,
    SequenceEvent,
    StepEvent,
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

} // namespace cv_sequencer
} // namespace dhe
