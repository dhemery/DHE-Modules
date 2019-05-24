#include "modules/Func.h"

namespace dhe {
Func::Func() {
  config(ParameterCount, InputCount, OutputCount);

  configKnob(OperandKnob, "Operand");
  configParam(OperatorSwitch, 0.F, 1.F, 0.F, "Operation");
  configParam(AdditionRangeSwitch, 0.F, 3.F, 1.F, "Addition operand range");
  configParam(MultiplicationRangeSwitch, 0.F, 3.F, 2.F, "Multiplication operand range");
  channel = FuncChannel{
      this, FuncInput, OperandKnob, FuncOutput, OperatorSwitch, AdditionRangeSwitch, MultiplicationRangeSwitch};
}

void Func::process(const ProcessArgs & /*args*/) { channel.apply(0.F); }
} // namespace dhe
