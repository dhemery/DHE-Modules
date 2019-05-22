#include <string>

#include "modules/Func6.h"

namespace dhe {
Func6::Func6() {
  config(PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT);
  for (int i = 0; i < channel_count; i++) {
    auto channelNumber = std::to_string(i + 1);

    configKnob(KNOB + i, "Operand " + channelNumber);
    configParam(OPERATOR_SWITCH + i, 0.f, 1.f, 0.f,
                "Operation " + channelNumber);
    configParam(ADDITION_RANGE_SWITCH + i, 0.f, 3.f, 1.f,
                "Addition operand " + channelNumber + " range");
    configParam(MULTIPLICATION_RANGE_SWITCH + i, 0.f, 3.f, 2.f,
                "Multiplication operand " + channelNumber + " range");

    channels.emplace_back(this, IN + i, KNOB + i, OUT + i, OPERATOR_SWITCH + i,
                          ADDITION_RANGE_SWITCH + i,
                          MULTIPLICATION_RANGE_SWITCH + i);
  }
}

void Func6::process(const ProcessArgs &args) {
  auto upstream = 0.f;
  for (auto &channel : channels) {
    upstream = channel.apply(upstream);
  }
}

} // namespace dhe
