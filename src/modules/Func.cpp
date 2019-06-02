#include "modules/Func.h"

namespace dhe {
Func::Func() :
    channel{this, FuncInput, AmountKnob, FuncOutput, OperationSwitch, OffsetRangeSwitch, MultiplierRangeSwitch} {
  config(ParameterCount, InputCount, OutputCount);

  channel.config();
}

void Func::process(const ProcessArgs & /*args*/) { channel.apply(0.F); }
} // namespace dhe
