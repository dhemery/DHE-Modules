#pragma once

namespace dhe {
namespace cv_sequencer {
auto constexpr step_count = 4;

struct ParamIds {
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
    StepDuration = StepCurvature + step_count,
    EnableStep = StepDuration + step_count,
    StepEndLevel = EnableStep + step_count,
    StepInterruptMode = StepEndLevel + step_count,
    StepTriggerMode = StepInterruptMode + step_count,
    StepShape = StepTriggerMode + step_count,
    StepStartLevel = StepShape + step_count,
    StepCompletionMode = StepStartLevel + step_count,
    StepStartAnchorSource = StepCompletionMode + step_count,
    StepEndAnchorSource = StepStartAnchorSource + step_count,
    StepStartAnchorMode = StepEndAnchorSource + step_count,
    StepEndAnchorMode = StepStartAnchorMode + step_count,
    Count = StepEndAnchorMode + step_count,
  };
};

struct InputIds {
  enum {
    In,
    Gate,
    Loop,
    Reset,
    Run,
    EnableStep,
    Aux = EnableStep + step_count,
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

struct LightIds {
  enum {
    StepProgress,
    Count = StepProgress + step_count + step_count,
  };
};

} // namespace cv_sequencer
} // namespace dhe
