#pragma once

#include "control-ids.h"

#include "controls/buttons.h"
#include "controls/switches.h"
#include "params/presets.h"
#include "signals/basic.h"
#include "signals/voltages.h"

#include "rack.hpp"

namespace dhe {

namespace fuzzy_logic {

template <typename TEngine> struct Module : public rack::engine::Module {
  Module() {
    config(ParamId::Count, InputId::Count, OutputId::Count);
    configInput(InputId::A + 0, "A");
    Button::config(this, ParamId::NegateA + 0, "Negate A");
    configInput(InputId::B + 0, "B");
    Button::config(this, ParamId::NegateB + 0, "Negate B");
    configInput(InputId::A + 1, "C");
    Button::config(this, ParamId::NegateA + 1, "Negate C");
    configInput(InputId::B + 1, "D");
    Button::config(this, ParamId::NegateB + 1, "Negate D");

    Switch::config<VoltageRanges>(this, ParamId::LevelRange, "Level Range",
                                  VoltageRangeId::Unipolar);

    configOutput(OutputId::And + 0, "A AND B");
    configOutput(OutputId::Nand + 0, "A NAND B");
    configOutput(OutputId::Or + 0, "A OR B");
    configOutput(OutputId::Nor + 0, "A NOR B");
    configOutput(OutputId::Xor + 0, "A XOR B");
    configOutput(OutputId::Xnor + 0, "A XNOR B");
    configOutput(OutputId::Implication + 0, "A implies B");
    configOutput(OutputId::Nonimplication + 0, "A does not imply B");
    configOutput(OutputId::ConverseImplication + 0, "B implies A");
    configOutput(OutputId::ConverseNonimplication + 0, "B does not imply A");

    configOutput(OutputId::And + 1, "C AND D");
    configOutput(OutputId::Nand + 1, "C NAND D");
    configOutput(OutputId::Or + 1, "C OR D");
    configOutput(OutputId::Nor + 1, "C NOR D");
    configOutput(OutputId::Xor + 1, "C XOR D");
    configOutput(OutputId::Xnor + 1, "C XNOR D");
    configOutput(OutputId::Implication + 1, "C implies D");
    configOutput(OutputId::Nonimplication + 1, "C does not imply D");
    configOutput(OutputId::ConverseImplication + 1, "D implies C");
    configOutput(OutputId::ConverseNonimplication + 1, "D does not imply C");
  }

  void process(ProcessArgs const & /*ignored*/) override {
    auto const voltage_offset = value_of(params[ParamId::LevelRange]) * 5.F;
    for (auto i = 0; i < 2; i++) {
      auto const a_input = inputs[InputId::A + i].getVoltage() + voltage_offset;
      auto const b_input = inputs[InputId::B + i].getVoltage() + voltage_offset;

      auto const a =
          is_pressed(params[(ParamId::NegateA + i)]) ? 10.F - a_input : a_input;
      auto const b =
          is_pressed(params[(ParamId::NegateB + i)]) ? 10.F - b_input : b_input;

      auto const out = TEngine::compute(a, b);

      set_outputs(OutputId::And + i, OutputId::Nand + i, out.a_and_b_,
                  voltage_offset);
      set_outputs(OutputId::Or + i, OutputId::Nor + i, out.a_or_b_,
                  voltage_offset);
      set_outputs(OutputId::Xor + i, OutputId::Xnor + i, out.a_xor_b_,
                  voltage_offset);
      set_outputs(OutputId::Implication + i, OutputId::Nonimplication + i,
                  out.a_implies_b_, voltage_offset);
      set_outputs(OutputId::ConverseImplication + i,
                  OutputId::ConverseNonimplication + i, out.b_implies_a_,
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
