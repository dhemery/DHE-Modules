#pragma once

namespace dhe {
namespace envelope {
namespace upstage {

struct ParamId {
  enum { Level, Trigger, Wait, LevelRange, Count };
};

struct InputId {
  enum { Trigger, Wait, LevelCv, Count };
};

struct OutputId {
  enum { Trigger, Envelope, Count };
};
} // namespace upstage
} // namespace envelope
} // namespace dhe
