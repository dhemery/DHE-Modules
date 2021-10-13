#pragma once
#include "./fuzzy-logic-controls.h"
#include "params/level-config.h"
#include "signals/common-inputs.h"

#include "rack.hpp"

namespace dhe {

namespace fuzzy_logic {

class FuzzyLogicZModule : public rack::engine::Module {
  using Controls = FuzzyLogicControls;

public:
  FuzzyLogicZModule() {
    config(Controls::ParameterCount, Controls::InputCount,
           Controls::OutputCount);

    configInput(Controls::AInputs + 0, "A");
    configParam(Controls::NotAButtons + 0, 0.F, 1.F, 0.F, "Negate A");
    configInput(Controls::BInputs + 0, "B");
    configParam(Controls::NotBButtons + 0, 0.F, 1.F, 0.F, "Negate B");
    configInput(Controls::AInputs + 1, "C");
    configParam(Controls::NotAButtons + 1, 0.F, 1.F, 0.F, "Negate C");
    configInput(Controls::BInputs + 1, "D");
    configParam(Controls::NotBButtons + 1, 0.F, 1.F, 0.F, "Negate D");

    config_level_range_switch(this, Controls::LevelRangeSwitch, "Level Range");

    configOutput(Controls::AndOutputs + 0, "A AND B");
    configOutput(Controls::NandOutputs + 0, "A NAND B");
    configOutput(Controls::OrOutputs + 0, "A OR B");
    configOutput(Controls::NorOutputs + 0, "A NOR B");
    configOutput(Controls::XorOutputs + 0, "A XOR B");
    configOutput(Controls::XnorOutputs + 0, "A XNOR B");
    configOutput(Controls::ImplicationOutputs + 0, "A implies B");
    configOutput(Controls::NonimplicationOutputs + 0, "A does not imply B");
    configOutput(Controls::ConverseImplicationOutputs + 0, "B implies A");
    configOutput(Controls::ConverseNonimplicationOutputs + 0,
                 "B does not imply A");

    configOutput(Controls::AndOutputs + 1, "C AND D");
    configOutput(Controls::NandOutputs + 1, "C NAND D");
    configOutput(Controls::OrOutputs + 1, "C OR D");
    configOutput(Controls::NorOutputs + 1, "C NOR D");
    configOutput(Controls::XorOutputs + 1, "C XOR D");
    configOutput(Controls::XnorOutputs + 1, "C XNOR D");
    configOutput(Controls::ImplicationOutputs + 1, "C implies D");
    configOutput(Controls::NonimplicationOutputs + 1, "C does not imply D");
    configOutput(Controls::ConverseImplicationOutputs + 1, "D implies C");
    configOutput(Controls::ConverseNonimplicationOutputs + 1,
                 "D does not imply C");
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

      auto const a_and_b = std::min(a, b);
      auto const a_or_b = std::max(a, b);
      auto const a_xor_b = a_or_b - a_and_b;
      auto const a_implies_b = std::max(not_a, b);
      auto const b_implies_a = std::max(a, not_b);

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

  auto dataToJson() -> json_t * override {
    auto *data = json_object();
    json_object_set_new(data, preset_version_key, json_integer(0));
    return data;
  }
};

} // namespace fuzzy_logic
} // namespace dhe
