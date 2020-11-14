#pragma once

#define MULTIPLE(name, n) name, name##_LAST = (name) + (n)-1

namespace dhe {
namespace truth {

template <int N> struct ParamIds {
  enum {
    MULTIPLE(Condition, N),
    MULTIPLE(InputOverride, N),
    MULTIPLE(Outcome, 1 << N),
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
    NotQ,
    Count,
  };
};

} // namespace truth
} // namespace dhe

#undef MULTIPLE
