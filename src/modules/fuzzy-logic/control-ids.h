#pragma once

#define TWO_OF(name) name, name##_LAST = ((name) + 1)

namespace dhe {
namespace fuzzy_logic {
struct Param {
  enum { TWO_OF(NegateA), TWO_OF(NegateB), LevelRange, Count };
};

struct Input {
  enum { TWO_OF(A), TWO_OF(B), Count };
};

struct Output {
  enum {
    TWO_OF(And),
    TWO_OF(Nand),
    TWO_OF(Or),
    TWO_OF(Nor),
    TWO_OF(Xor),
    TWO_OF(Xnor),
    TWO_OF(Implication),
    TWO_OF(Nonimplication),
    TWO_OF(ConverseImplication),
    TWO_OF(ConverseNonimplication),
    Count
  };
};
} // namespace fuzzy_logic
} // namespace dhe

#undef TWO_OF
