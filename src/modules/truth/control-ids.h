#pragma once

#define MULTIPLE(name, n) name, name##_LAST = (name) + (n)-1

namespace dhe {
namespace truth {

template <int N> struct ParamIds {
  enum {
    ForcQHigh,
    GateMode,
    MULTIPLE(ForceInputHigh, N),
    MULTIPLE(Outcome, 1 << N),
    ForceQNotHigh,
    Count,
  };
};

template <int N> struct InputIds {
  enum {
    MULTIPLE(Input, N),
    Count,
  };
};

struct OutputIds {
  enum {
    Q,
    QNot,
    Count,
  };
};

} // namespace truth
} // namespace dhe

#undef MULTIPLE
