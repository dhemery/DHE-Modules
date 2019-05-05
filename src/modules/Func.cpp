#include "modules/Func.h"

namespace DHE {
Func::Func() {
  config(PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT);
  channel = std::unique_ptr<FuncChannel>(new FuncChannel(this, IN, KNOB, OUT));

  configKnob(KNOB, "Operand");
  configParam(OPERATOR_SWITCH, 0.f, 1.f, 0.f, "Operation");
  configParam(ADDITION_RANGE_SWITCH, 0.f, 3.f, 0.f, "Addition operand range");
  configParam(MULTIPLICATION_RANGE_SWITCH, 0.f, 3.f, 0.f,
              "Multiplication operand range");
}

void Func::process(const ProcessArgs &args) { channel->adjust(0.f); }
} // namespace DHE
