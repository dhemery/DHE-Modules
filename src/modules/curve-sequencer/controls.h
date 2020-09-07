#pragma once

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
    // Above: Overall module/sequence params
    // Below: Step params
    StepCurvature,
    StepDuration = StepCurvature + N,
    EnableStep = StepDuration + N,
    StepEndLevel = EnableStep + N,
    StepTriggerMode = StepEndLevel + N,
    StepInterruptMode = StepTriggerMode + N,
    StepShape = StepInterruptMode + N,
    // The rest are new in 1.3.0
    StepStartLevel = StepShape + N,
    StepCompletionMode = StepStartLevel + N,
    StepStartAnchorSource = StepCompletionMode + N,
    StepEndAnchorSource = StepStartAnchorSource + N,
    StepStartAnchorMode = StepEndAnchorSource + N,
    StepEndAnchorMode = StepStartAnchorMode + N,
    Count = StepEndAnchorMode + N,
  };
};

// How obsolete v1.1.0 parameter IDs map to v1.3 IDs
template <int N> struct V110ParamIds {
  enum {
    LevelKnobs = ParamIds<N>::StepEndLevel,
    ModeSwitches = ParamIds<N>::StepTriggerMode,
    ConditionSwitches = ParamIds<N>::StepInterruptMode,
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
    LevelCV,
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

} // namespace curve_sequencer
} // namespace dhe