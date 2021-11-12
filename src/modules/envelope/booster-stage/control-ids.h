#pragma once

namespace dhe {
namespace envelope {
namespace booster_stage {

struct ParamId {
  enum {
    Active,
    Curvature,
    Defer,
    Duration,
    DurationRange,
    Eoc,
    Level,
    LevelRange,
    Shape,
    Trigger,
    Count
  };
};

struct InputId {
  enum { CurvatureCv, Defer, DurationCv, LevelCv, Envelope, Trigger, Count };
};

struct OutputId {
  enum { Active, Eoc, Envelope, Count };
};
} // namespace booster_stage
} // namespace envelope
} // namespace dhe
