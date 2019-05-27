#include "modules/Func.h"

namespace dhe {
Func::Func() {
  config(ParameterCount, InputCount, OutputCount);

  channel
      = FuncChannel{this, FuncInput, AmountKnob, FuncOutput, OperationSwitch, OffsetRangeSwitch, MultiplierRangeSwitch};
}

void Func::process(const ProcessArgs & /*args*/) { channel.apply(0.F); }
} // namespace dhe
