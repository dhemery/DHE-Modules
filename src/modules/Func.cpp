#include "modules/Func.h"

#include "modules/controls/Controls.h"

namespace dhe {
Func::Func() {
  config(ParameterCount, InputCount, OutputCount);

  toggle::config<2>(this, OperationSwitch, "Operation", {"Add (offset)", "Multiply (scale)"}, 0);
  configKnob(OperandKnob, "Amount");
  toggle::config<4>(this, AddendRangeSwitch, "Offset range", {"0–5 V", "±5 V", "0–10 V", "±10 V"}, 1);
  toggle::config<4>(this, MultiplicandRangeSwitch, "Multiplier range", {"0–1", "±1", "0–2", "±2"}, 2);
  channel = FuncChannel{
      this, FuncInput, OperandKnob, FuncOutput, OperationSwitch, AddendRangeSwitch, MultiplicandRangeSwitch};
}

void Func::process(const ProcessArgs & /*args*/) { channel.apply(0.F); }
} // namespace dhe
