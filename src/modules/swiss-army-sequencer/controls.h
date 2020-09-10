#pragma once

namespace dhe {
namespace swiss_army_sequencer {
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
    StepInterruptMode = StepEndLevel + N,    // In v0: generate mode
    StepTriggerMode = StepInterruptMode + N, // In v0: advance mode
    StepShape = StepTriggerMode + N,
    // The rest are new in json v1 (DHE-Modules 1.3.0):
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
    In,
    Gate,
    Loop,
    Reset,
    Run,
    EnableStep,
    Aux = EnableStep + N,
    DurationCV,
    Count,
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

} // namespace swiss_army_sequencer
} // namespace dhe
