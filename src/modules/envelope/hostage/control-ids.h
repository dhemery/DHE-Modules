#pragma once

namespace dhe {
namespace envelope {
namespace hostage {
struct ParamId {
  enum { Duration, DurationRange, Mode, Count };
};

struct InputId {
  enum { Defer, DurationCv, Envelope, Trigger, Count };
};

struct OutputId {
  enum { Active, Envelope, Eoc, Count };
};

} // namespace hostage
} // namespace envelope
} // namespace dhe
