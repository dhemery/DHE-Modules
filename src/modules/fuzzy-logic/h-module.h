#pragma once
#include "config/level-config.h"
#include "controls.h"
#include "controls/common-inputs.h"

#include <engine/Module.hpp>

namespace dhe {

namespace fuzzy_logic {

class HModule : public rack::engine::Module {
  using Controls = Controls;

public:
  HModule() {
    config(Controls::ParameterCount, Controls::InputCount,
           Controls::OutputCount);
    configParam(Controls::NotAButtons + 0, 0.F, 1.F, 0.F, "Negate A");
    configParam(Controls::NotBButtons + 0, 0.F, 1.F, 0.F, "Negate B");
    configParam(Controls::NotAButtons + 1, 0.F, 1.F, 0.F, "Negate C");
    configParam(Controls::NotBButtons + 1, 0.F, 1.F, 0.F, "Negate D");
    config_level_range_switch(this, Controls::LevelRangeSwitch, "Level Range");
  }

  void process(ProcessArgs const & /*ignored*/) override {
    auto const voltage_offset =
        position_of(params[Controls::LevelRangeSwitch]) == 1 ? 0.F : 5.F;
    for (auto i = 0; i < 2; i++) {
      auto const a_input =
          inputs[Controls::AInputs + i].getVoltage() + voltage_offset;
      auto const b_input =
          inputs[Controls::BInputs + i].getVoltage() + voltage_offset;
      auto const a = is_pressed(params[(Controls::NotAButtons + i)])
                         ? 10.F - a_input
                         : a_input;
      auto const not_a = 10.F - a;
      auto const b = is_pressed(params[(Controls::NotBButtons + i)])
                         ? 10.F - b_input
                         : b_input;
      auto const not_b = 10.F - b;

      auto const a_and_b = a * b * 0.1F;
      auto const a_or_b = a + b - a_and_b;
      auto const a_xor_b = a_or_b - a_and_b;
      auto const a_implies_b = not_a + a_and_b;
      auto const b_implies_a = not_b + a_and_b;

      set_outputs(Controls::AndOutputs + i, Controls::NandOutputs + i, a_and_b,
                  voltage_offset);
      set_outputs(Controls::OrOutputs + i, Controls::NorOutputs + i, a_or_b,
                  voltage_offset);
      set_outputs(Controls::XorOutputs + i, Controls::XnorOutputs + i, a_xor_b,
                  voltage_offset);
      set_outputs(Controls::ImplicationOutputs + i,
                  Controls::NonimplicationOutputs + i, a_implies_b,
                  voltage_offset);
      set_outputs(Controls::ConverseImplicationOutputs + i,
                  Controls::ConverseNonimplicationOutputs + i, b_implies_a,
                  voltage_offset);
    }
  }

  void set_outputs(int output_id, int negated_output_id, float voltage,
                   float offset) {
    outputs[output_id].setVoltage(voltage - offset);
    outputs[negated_output_id].setVoltage(10.F - voltage - offset);
  }
};
} // namespace fuzzy_logic
} // namespace dhe
