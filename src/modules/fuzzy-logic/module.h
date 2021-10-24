#pragma once
#include "control-ids.h"

#include "params/level-config.h"
#include "signals/common-inputs.h"

#include "rack.hpp"

namespace dhe {

namespace fuzzy_logic {

template <typename TEngine> struct Module : public rack::engine::Module {
  Module() {
    config(Param::ParameterCount, Input::InputCount, Output::OutputCount);
    configInput(Input::AInputs + 0, "A");
    configParam(Param::NotAButtons + 0, 0.F, 1.F, 0.F, "Negate A");
    configInput(Input::BInputs + 0, "B");
    configParam(Param::NotBButtons + 0, 0.F, 1.F, 0.F, "Negate B");
    configInput(Input::AInputs + 1, "C");
    configParam(Param::NotAButtons + 1, 0.F, 1.F, 0.F, "Negate C");
    configInput(Input::BInputs + 1, "D");
    configParam(Param::NotBButtons + 1, 0.F, 1.F, 0.F, "Negate D");

    config_level_range_switch(this, Param::LevelRangeSwitch, "Level Range");

    configOutput(Output::AndOutputs + 0, "A AND B");
    configOutput(Output::NandOutputs + 0, "A NAND B");
    configOutput(Output::OrOutputs + 0, "A OR B");
    configOutput(Output::NorOutputs + 0, "A NOR B");
    configOutput(Output::XorOutputs + 0, "A XOR B");
    configOutput(Output::XnorOutputs + 0, "A XNOR B");
    configOutput(Output::ImplicationOutputs + 0, "A implies B");
    configOutput(Output::NonimplicationOutputs + 0, "A does not imply B");
    configOutput(Output::ConverseImplicationOutputs + 0, "B implies A");
    configOutput(Output::ConverseNonimplicationOutputs + 0,
                 "B does not imply A");

    configOutput(Output::AndOutputs + 1, "C AND D");
    configOutput(Output::NandOutputs + 1, "C NAND D");
    configOutput(Output::OrOutputs + 1, "C OR D");
    configOutput(Output::NorOutputs + 1, "C NOR D");
    configOutput(Output::XorOutputs + 1, "C XOR D");
    configOutput(Output::XnorOutputs + 1, "C XNOR D");
    configOutput(Output::ImplicationOutputs + 1, "C implies D");
    configOutput(Output::NonimplicationOutputs + 1, "C does not imply D");
    configOutput(Output::ConverseImplicationOutputs + 1, "D implies C");
    configOutput(Output::ConverseNonimplicationOutputs + 1,
                 "D does not imply C");
  }

  void process(ProcessArgs const & /*ignored*/) override {
    auto const voltage_offset =
        position_of(params[Param::LevelRangeSwitch]) == 1 ? 0.F : 5.F;
    for (auto i = 0; i < 2; i++) {
      auto const a_input =
          inputs[Input::AInputs + i].getVoltage() + voltage_offset;
      auto const b_input =
          inputs[Input::BInputs + i].getVoltage() + voltage_offset;

      auto const a = is_pressed(params[(Param::NotAButtons + i)])
                         ? 10.F - a_input
                         : a_input;
      auto const b = is_pressed(params[(Param::NotBButtons + i)])
                         ? 10.F - b_input
                         : b_input;

      auto const out = TEngine::compute(a, b);

      set_outputs(Output::AndOutputs + i, Output::NandOutputs + i, out.a_and_b_,
                  voltage_offset);
      set_outputs(Output::OrOutputs + i, Output::NorOutputs + i, out.a_or_b_,
                  voltage_offset);
      set_outputs(Output::XorOutputs + i, Output::XnorOutputs + i, out.a_xor_b_,
                  voltage_offset);
      set_outputs(Output::ImplicationOutputs + i,
                  Output::NonimplicationOutputs + i, out.a_implies_b_,
                  voltage_offset);
      set_outputs(Output::ConverseImplicationOutputs + i,
                  Output::ConverseNonimplicationOutputs + i, out.b_implies_a_,
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
