#pragma once

#include "control-ids.h"

#include "signals/basic.h"

#include "rack.hpp"

#include <string>

namespace dhe {

namespace gator {

class Module : public rack::engine::Module {
public:
  Module() {
    config(ParamId::Count, InputId::Count, OutputId::Count);
    for (auto i = 0; i < InputId::Count; i++) {
      auto const signal_name = std::to_string(i + 1);
      configInput(InputId::Signal + i, "Signal " + signal_name);
      Button::config(this, ParamId::NegateSignal + i,
                     "Negate signal " + signal_name);

      configOutput(OutputId::And, "AND");
      configOutput(OutputId::Nand, "NAND");
      configOutput(OutputId::Or, "OR");
      configOutput(OutputId::Nor, "NOR");
      configOutput(OutputId::Even, "Even");
      configOutput(OutputId::Odd, "Odd");
      configOutput(OutputId::Xor, "XOR");
      configOutput(OutputId::Xnor, "XNOR");
    }
  }

  void process(ProcessArgs const & /*ignored*/) override {
    auto connected_count = 0;
    auto high_count = 0;

    for (auto i = 0; i < InputId::Count; i++) {
      if (inputs[i].isConnected()) {
        connected_count++;
        if (is_high(inputs[(InputId::Signal + i)]) !=
            is_pressed(params[ParamId::NegateSignal + i])) {
          high_count++;
        }
      }
    }

    if (connected_count == 0) {
      set_all_outputs_false();
    } else {
      set_outputs(OutputId::And, OutputId::Nand, high_count == connected_count);
      set_outputs(OutputId::Or, OutputId::Nor, high_count > 0);
      set_outputs(OutputId::Odd, OutputId::Even, (high_count & 1) > 0);
      set_outputs(OutputId::Xor, OutputId::Xnor, high_count == 1);
    }
  }

  auto dataToJson() -> json_t * override {
    auto *data = json_object();
    json_object_set_new(data, preset_version_key, json_integer(0));
    return data;
  }

private:
  void set_all_outputs_false() {
    for (auto i = 0; i < OutputId::Count; i++) {
      outputs[i].setVoltage(0.F);
    }
  }

  void set_outputs(int output_id, int negated_output_id, bool value) {
    auto const voltage = value ? 10.F : 0.F;
    outputs[output_id].setVoltage(voltage);
    outputs[negated_output_id].setVoltage(10.F - voltage);
  }
};

} // namespace gator
} // namespace dhe
