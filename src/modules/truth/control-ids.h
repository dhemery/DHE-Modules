#pragma once

#define MULTIPLE(name, n) name, name##_LAST = (name) + (n)-1

namespace dhe {
namespace truth {

template <int N> struct ParamIds {
  enum {
    Input0Selector,             // In v0: Input0Selection
    QOverride = Input0Selector, // In v1: QOverride
    GateMode,
    MULTIPLE(InputOverride, N),
    MULTIPLE(Outcome, 1 << N),
    QNotOverride,
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
