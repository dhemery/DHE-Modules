#pragma once

namespace dhe {
namespace envelope {
namespace stage {
struct Param {
  enum { DurationKnob, LevelKnob, CurveKnob, ParameterCount };
};

struct Input {
  enum { EnvelopeInput, TriggerInput, DeferInput, InputCount };
};

struct Output {
  enum { EnvelopeOutput, EocOutput, ActiveOutput, OutputCount };
};
} // namespace stage
} // namespace envelope
} // namespace dhe
