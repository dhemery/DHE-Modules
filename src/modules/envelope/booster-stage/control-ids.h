#pragma once

namespace dhe {
namespace envelope {
namespace booster_stage {

struct Param {
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

struct Input {
  enum { CurvatureCv, Defer, DurationCv, LevelCv, Envelope, Trigger, Count };
};

struct Output {
  enum { Active, Eoc, Envelope, Count };
};
} // namespace booster_stage
} // namespace envelope
} // namespace dhe
