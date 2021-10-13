#pragma once

#include "./gator-controls.h"
#include "signals/common-inputs.h"

#include "rack.hpp"

#include <string>

namespace dhe {

namespace gator {

class Gator : public rack::engine::Module {
  using Controls = GatorControls;

public:
  Gator() {
    config(Controls::ParameterCount, Controls::InputCount,
           Controls::OutputCount);
    for (auto i = 0; i < Controls::InputCount; i++) {
      auto const signal_name = std::to_string(i + 1);
      configInput(Controls::Inputs + i, "Signal " + signal_name);
      configButton(Controls::NegateButtons + i, "Negate signal " + signal_name);

      configOutput(Controls::AndOutput, "AND");
      configOutput(Controls::NandOutput, "NAND");
      configOutput(Controls::OrOutput, "OR");
      configOutput(Controls::NorOutput, "NOR");
      configOutput(Controls::EvenOutput, "Even");
      configOutput(Controls::OddOutput, "Odd");
      configOutput(Controls::XorOutput, "XOR");
      configOutput(Controls::XnorOutput, "XNOR");
    }
  }

  void process(ProcessArgs const & /*ignored*/) override {
    auto connected_count = 0;
    auto high_count = 0;

    for (auto i = 0; i < Controls::InputCount; i++) {
      if (inputs[i].isConnected()) {
        connected_count++;
        if (is_high(inputs[(Controls::Inputs + i)]) !=
            is_pressed(params[Controls::NegateButtons + i])) {
          high_count++;
        }
      }
    }

    if (connected_count == 0) {
      set_all_outputs_false();
    } else {
      set_outputs(Controls::AndOutput, Controls::NandOutput,
                  high_count == connected_count);
      set_outputs(Controls::OrOutput, Controls::NorOutput, high_count > 0);
      set_outputs(Controls::OddOutput, Controls::EvenOutput,
                  (high_count & 1) > 0);
      set_outputs(Controls::XorOutput, Controls::XnorOutput, high_count == 1);
    }
  }

  auto dataToJson() -> json_t * override {
    auto *data = json_object();
    json_object_set_new(data, preset_version_key, json_integer(0));
    return data;
  }

private:
  void set_all_outputs_false() {
    for (auto i = 0; i < Controls::OutputCount; i++) {
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
