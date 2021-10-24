#pragma once

namespace dhe {
namespace envelope {
namespace hostage {
struct Param {
  enum { Duration, DurationRange, Mode, Count };
};

struct Input {
  enum { Defer, DurationCv, Envelope, Trigger, Count };
};

struct Output {
  enum { Active, Envelope, Eoc, Count };
};

} // namespace hostage
} // namespace envelope
} // namespace dhe
