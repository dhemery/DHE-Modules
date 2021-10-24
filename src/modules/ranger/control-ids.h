#pragma once

namespace dhe {
namespace ranger {
struct Param {
  enum {
    Level,
    LevelAv,
    CcwLimit,
    CcwLimitAv,
    CcwLimitRange,
    CwLimit,
    CwLimitAv,
    CwLimitRange,
    Count
  };
};

struct Input {
  enum { LevelCv, CcwLimitCv, CwLimitCv, Count };
};

struct Output {
  enum { Main, Count };
};
} // namespace ranger
} // namespace dhe
