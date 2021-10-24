#pragma once

#define PER_CHANNEL(name, n) name, name##_LAST = ((name) + (n)-1)

namespace dhe {
namespace func {
template <int N> struct ParamIds {
  enum {
    PER_CHANNEL(Operand, N),
    PER_CHANNEL(Operation, N),
    PER_CHANNEL(OffsetRange, N),
    PER_CHANNEL(MultiplierRange, N),
    Count
  };
};

template <int N> struct InputIds {
  enum { PER_CHANNEL(Channel, N), Count };
};

template <int N> struct OutputIds {
  enum { PER_CHANNEL(Channel, N), Count };
};

} // namespace func
} // namespace dhe

#undef PER_CHANNEL
