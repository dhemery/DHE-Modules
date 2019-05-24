#include "modules/Func6.h"

#include <string>

namespace dhe {
Func6::Func6() {
  config(ParameterCount, InputCount, OutputCount);
  for (int i = 0; i < channelCount; i++) {
    auto channelNumber = std::to_string(i + 1);

    configKnob(OperandKnob + i, "Operand " + channelNumber);
    configParam(OperatorSwitch + i, 0.F, 1.F, 0.F, "Operation " + channelNumber);
    configParam(AdditionRangeSwitch + i, 0.F, 3.F, 1.F, "Addition operand " + channelNumber + " range");
    configParam(MultiplicationRangeSwitch + i, 0.F, 3.F, 2.F, "Multiplication operand " + channelNumber + " range");

    channels.emplace_back(this, FuncInput + i, OperandKnob + i, FuncOutput + i, OperatorSwitch + i,
                          AdditionRangeSwitch + i, MultiplicationRangeSwitch + i);
  }
}

void Func6::process(const ProcessArgs & /*ignored*/) {
  auto upstream = 0.F;
  for (auto &channel : channels) {
    upstream = channel.apply(upstream);
  }
}

} // namespace dhe
