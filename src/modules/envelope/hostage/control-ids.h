#pragma once

namespace dhe {
namespace envelope {
namespace hostage {
struct Param {
  enum { DurationKnob, DurationRangeSwitch, ModeSwitch, ParameterCount };
};

struct Input {
  enum { DeferInput, DurationCvInput, EnvelopeInput, TriggerInput, InputCount };
};

struct Output {
  enum { ActiveOutput, EnvelopeOutput, EocOutput, OutputCount };
};

} // namespace hostage
} // namespace envelope
} // namespace dhe
