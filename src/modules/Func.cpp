#include "modules/Func.h"

namespace DHE {
Func::Func() {
  config(PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT);

  configKnob(KNOB, "Operand");
  configParam(OPERATOR_SWITCH, 0.f, 1.f, 0.f, "Operation");
  configParam(ADDITION_RANGE_SWITCH, 0.f, 3.f, 1.f, "Addition operand range");
  configParam(MULTIPLICATION_RANGE_SWITCH, 0.f, 3.f, 2.f,
              "Multiplication operand range");
  channel = std::unique_ptr<FuncChannel>(
      new FuncChannel(this, IN, KNOB, OUT, OPERATOR_SWITCH,
                      ADDITION_RANGE_SWITCH, MULTIPLICATION_RANGE_SWITCH));
}

void Func::process(const ProcessArgs &args) { channel->apply(0.f); }
} // namespace DHE
