#pragma once

namespace dhe {
namespace envelope {
namespace booster_stage {

struct Param {
  enum {
    ActiveButton,
    CurveKnob,
    DeferButton,
    DurationKnob,
    DurationRangeSwitch,
    EocButton,
    LevelKnob,
    LevelRangeSwitch,
    ShapeSwitch,
    TriggerButton,
    ParameterCount
  };
};

struct Input {
  enum {
    CurveCvInput,
    DeferInput,
    DurationCvInput,
    LevelCvInput,
    EnvelopeInput,
    TriggerInput,
    InputCount
  };
};

struct Output {
  enum { ActiveOutput, EocOutput, EnvelopeOutput, OutputCount };
};
} // namespace booster_stage
} // namespace envelope
} // namespace dhe
