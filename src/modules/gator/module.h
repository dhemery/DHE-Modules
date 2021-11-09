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
    config(Param::Count, Input::Count, Output::Count);
    for (auto i = 0; i < Input::Count; i++) {
      auto const signal_name = std::to_string(i + 1);
      configInput(Input::Signal + i, "Signal " + signal_name);
      Button::config(this, Param::NegateSignal + i,
                     "Negate signal " + signal_name);

      configOutput(Output::And, "AND");
      configOutput(Output::Nand, "NAND");
      configOutput(Output::Or, "OR");
      configOutput(Output::Nor, "NOR");
      configOutput(Output::Even, "Even");
      configOutput(Output::Odd, "Odd");
      configOutput(Output::Xor, "XOR");
      configOutput(Output::Xnor, "XNOR");
    }
  }

  void process(ProcessArgs const & /*ignored*/) override {
    auto connected_count = 0;
    auto high_count = 0;

    for (auto i = 0; i < Input::Count; i++) {
      if (inputs[i].isConnected()) {
        connected_count++;
        if (is_high(inputs[(Input::Signal + i)]) !=
            is_pressed(params[Param::NegateSignal + i])) {
          high_count++;
        }
      }
    }

    if (connected_count == 0) {
      set_all_outputs_false();
    } else {
      set_outputs(Output::And, Output::Nand, high_count == connected_count);
      set_outputs(Output::Or, Output::Nor, high_count > 0);
      set_outputs(Output::Odd, Output::Even, (high_count & 1) > 0);
      set_outputs(Output::Xor, Output::Xnor, high_count == 1);
    }
  }

  auto dataToJson() -> json_t * override {
    auto *data = json_object();
    json_object_set_new(data, preset_version_key, json_integer(0));
    return data;
  }

private:
  void set_all_outputs_false() {
    for (auto i = 0; i < Output::Count; i++) {
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
