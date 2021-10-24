#pragma once

namespace dhe {
namespace envelope {
namespace upstage {

struct Param {
  enum { Level, Trigger, Wait, LevelRange, Count };
};

struct Input {
  enum { Trigger, Wait, LevelCv, Count };
};

struct Output {
  enum { Trigger, Envelope, Count };
};
} // namespace upstage
} // namespace envelope
} // namespace dhe
