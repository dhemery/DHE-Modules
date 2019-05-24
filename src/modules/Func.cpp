#include "modules/Func.h"

namespace dhe {
Func::Func() {
  config(ParameterCount, InputCount, OutputCount);

  configKnob(OperandKnob, "Operand");
  configParam(OperatorSwitch, 0.f, 1.f, 0.f, "Operation");
  configParam(AdditionRangeSwitch, 0.f, 3.f, 1.f, "Addition operand range");
  configParam(MultiplicationRangeSwitch, 0.f, 3.f, 2.f, "Multiplication operand range");
  channel = FuncChannel{
      this, FuncInput, OperandKnob, FuncOutput, OperatorSwitch, AdditionRangeSwitch, MultiplicationRangeSwitch};
}

void Func::process(const ProcessArgs & /*args*/) { channel.apply(0.f); }
} // namespace dhe
