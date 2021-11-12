#pragma once

namespace dhe {
namespace envelope {
namespace stage {
struct ParamId {
  enum { Duration, Level, Curvature, Count };
};

struct InputId {
  enum { Envelope, Trigger, Defer, Count };
};

struct OutputId {
  enum { Envelope, Eoc, Active, Count };
};
} // namespace stage
} // namespace envelope
} // namespace dhe
