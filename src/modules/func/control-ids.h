#pragma once

#define PER_CHANNEL(name, n) name, name##_LAST = ((name) + (n)-1)

namespace dhe {
namespace func {
template <int N> struct ParamIds {
  enum {
    PER_CHANNEL(AmountKnob, N),
    PER_CHANNEL(OperationSwitch, N),
    PER_CHANNEL(OffsetRangeSwitch, N),
    PER_CHANNEL(MultiplierRangeSwitch, N),
    ParameterCount
  };
};

template <int N> struct InputIds {
  enum { PER_CHANNEL(FuncInput, N), InputCount };
};

template <int N> struct OutputIds {
  enum { PER_CHANNEL(FuncOutput, N), OutputCount };
};

} // namespace func
} // namespace dhe

#undef PER_CHANNEL
