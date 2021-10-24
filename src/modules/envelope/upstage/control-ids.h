#pragma once

namespace dhe {
namespace envelope {
namespace upstage {

struct Param {
  enum {
    LevelKnob,
    TriggerButton,
    WaitButton,
    LevelRangeSwitch,
    ParameterCount
  };
};

struct Input {
  enum { TriggerInput, WaitInput, LevelCvInput, InputCount };
};

struct Output {
  enum { TriggerOutput, EnvelopeOutput, OutputCount };
};
} // namespace upstage
} // namespace envelope
} // namespace dhe
