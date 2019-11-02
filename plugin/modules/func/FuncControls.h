#pragma once

#include <common.hpp>

namespace dhe {
template <int N> class FuncControls {
public:
  enum ParameterIds {
    ENUMS(AmountKnob, N),
    ENUMS(OperationSwitch, N),
    ENUMS(OffsetRangeSwitch, N),
    ENUMS(MultiplierRangeSwitch, N),
    ParameterCount
  };

  enum InputIds { FuncInput, InputCount = FuncInput + N };

  enum OutputIds { FuncOutput, OutputCount = FuncOutput + N };
};
} // namespace dhe
