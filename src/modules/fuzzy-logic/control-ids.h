#pragma once

#define TWO_OF(name) name, name##_LAST = ((name) + 1)

namespace dhe {
namespace fuzzy_logic {
struct Param {
  enum {
    TWO_OF(NotAButtons),
    TWO_OF(NotBButtons),
    LevelRangeSwitch,
    ParameterCount
  };
};

struct Input {
  enum { TWO_OF(AInputs), TWO_OF(BInputs), InputCount };
};

struct Output {
  enum {
    TWO_OF(AndOutputs),
    TWO_OF(NandOutputs),
    TWO_OF(OrOutputs),
    TWO_OF(NorOutputs),
    TWO_OF(XorOutputs),
    TWO_OF(XnorOutputs),
    TWO_OF(ImplicationOutputs),
    TWO_OF(NonimplicationOutputs),
    TWO_OF(ConverseImplicationOutputs),
    TWO_OF(ConverseNonimplicationOutputs),
    OutputCount
  };
};
} // namespace fuzzy_logic
} // namespace dhe

#undef TWO_OF
