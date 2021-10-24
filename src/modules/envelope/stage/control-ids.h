#pragma once

namespace dhe {
namespace envelope {
namespace stage {
struct Param {
  enum { Duration, Level, Curvature, Count };
};

struct Input {
  enum { Envelope, Trigger, Defer, Count };
};

struct Output {
  enum { Envelope, Eoc, Active, Count };
};
} // namespace stage
} // namespace envelope
} // namespace dhe
