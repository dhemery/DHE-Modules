#include "modules/Func.h"

#include "modules/controls/ToggleControls.h"

namespace dhe {
Func::Func() {
  config(ParameterCount, InputCount, OutputCount);

  toggle::config<2>(this, OperationSwitch, "Operation", {"Add", "Multiply"});
  configKnob(OperandKnob, "Operand");
  configParam(AdditionRangeSwitch, 0.F, 3.F, 1.F, "Addition operand range");
  configParam(MultiplicationRangeSwitch, 0.F, 3.F, 2.F, "Multiplication operand range");
  channel = FuncChannel{
      this, FuncInput, OperandKnob, FuncOutput, OperationSwitch, AdditionRangeSwitch, MultiplicationRangeSwitch};
}

void Func::process(const ProcessArgs & /*args*/) { channel.apply(0.F); }
} // namespace dhe
