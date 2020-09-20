#pragma once

namespace dhe {
namespace cv_sequencer {

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
    StepCurvature,
    StepDuration = StepCurvature + N,
    EnableStep = StepDuration + N,
    StepEndLevel = EnableStep + N,
    StepInterruptMode = StepEndLevel + N,
    StepTriggerMode = StepInterruptMode + N,
    StepShape = StepTriggerMode + N,
    StepStartLevel = StepShape + N,
    StepCompletionMode = StepStartLevel + N,
    StepStartAnchorSource = StepCompletionMode + N,
    StepEndAnchorSource = StepStartAnchorSource + N,
    StepStartAnchorMode = StepEndAnchorSource + N,
    StepEndAnchorMode = StepStartAnchorMode + N,
    Count = StepEndAnchorMode + N,
  };
};

template <int N> struct InputIds {
  enum {
    Gate,
    Loop,
    Reset,
    Run,
    DurationCV,
    InA,
    InB,
    EnableStep,
    Count = EnableStep + N,
  };
};

struct OutputIds {
  enum {
    Out,
    Count,
  };
};

template <int N> struct LightIds {
  enum {
    StepProgress,
    Count = StepProgress + N + N,
  };
};

} // namespace cv_sequencer
} // namespace dhe
