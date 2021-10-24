#pragma once

#define PER_INPUT(name) name, name##_LAST = ((name) + (input_count)-1)

namespace dhe {
namespace gator {
static auto constexpr input_count = 16;

struct Param {
  enum { PER_INPUT(NegateSignal), Count };
};

struct Input {
  enum { PER_INPUT(Signal), Count };
};

struct Output {
  enum { And, Nand, Or, Nor, Even, Odd, Xor, Xnor, Count };
};
} // namespace gator
} // namespace dhe

#undef PER_INPUT
