#pragma once

namespace dhe {
namespace ranger {
struct ParamId {
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

struct InputId {
  enum { LevelCv, CcwLimitCv, CwLimitCv, Count };
};

struct OutputId {
  enum { Main, Count };
};
} // namespace ranger
} // namespace dhe
