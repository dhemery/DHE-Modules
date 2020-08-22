#pragma once

#include "controls/common-inputs.h"
#include "gator-controls.h"

#include <engine/Module.hpp>
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
      configParam(Controls::NegateButtons + i, 0.F, 1.F, 0.F,
                  "Negate input " + std::to_string(i + 1));
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
