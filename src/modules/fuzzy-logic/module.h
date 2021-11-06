#pragma once

#include "control-ids.h"

#include "controls/switches.h"
#include "signals/common-inputs.h"
#include "signals/levels.h"

#include "rack.hpp"

namespace dhe {

namespace fuzzy_logic {

template <typename TEngine> struct Module : public rack::engine::Module {
  Module() {
    config(Param::Count, Input::Count, Output::Count);
    configInput(Input::A + 0, "A");
    configParam(Param::NegateA + 0, 0.F, 1.F, 0.F, "Negate A");
    configInput(Input::B + 0, "B");
    configParam(Param::NegateB + 0, 0.F, 1.F, 0.F, "Negate B");
    configInput(Input::A + 1, "C");
    configParam(Param::NegateA + 1, 0.F, 1.F, 0.F, "Negate C");
    configInput(Input::B + 1, "D");
    configParam(Param::NegateB + 1, 0.F, 1.F, 0.F, "Negate D");

    ItemSwitch::config<Levels>(this, Param::LevelRange, "Level Range",
                               Levels::Unipolar);

    configOutput(Output::And + 0, "A AND B");
    configOutput(Output::Nand + 0, "A NAND B");
    configOutput(Output::Or + 0, "A OR B");
    configOutput(Output::Nor + 0, "A NOR B");
    configOutput(Output::Xor + 0, "A XOR B");
    configOutput(Output::Xnor + 0, "A XNOR B");
    configOutput(Output::Implication + 0, "A implies B");
    configOutput(Output::Nonimplication + 0, "A does not imply B");
    configOutput(Output::ConverseImplication + 0, "B implies A");
    configOutput(Output::ConverseNonimplication + 0, "B does not imply A");

    configOutput(Output::And + 1, "C AND D");
    configOutput(Output::Nand + 1, "C NAND D");
    configOutput(Output::Or + 1, "C OR D");
    configOutput(Output::Nor + 1, "C NOR D");
    configOutput(Output::Xor + 1, "C XOR D");
    configOutput(Output::Xnor + 1, "C XNOR D");
    configOutput(Output::Implication + 1, "C implies D");
    configOutput(Output::Nonimplication + 1, "C does not imply D");
    configOutput(Output::ConverseImplication + 1, "D implies C");
    configOutput(Output::ConverseNonimplication + 1, "D does not imply C");
  }

  void process(ProcessArgs const & /*ignored*/) override {
    auto const voltage_offset =
        position_of(params[Param::LevelRange]) == 1 ? 0.F : 5.F;
    for (auto i = 0; i < 2; i++) {
      auto const a_input = inputs[Input::A + i].getVoltage() + voltage_offset;
      auto const b_input = inputs[Input::B + i].getVoltage() + voltage_offset;

      auto const a =
          is_pressed(params[(Param::NegateA + i)]) ? 10.F - a_input : a_input;
      auto const b =
          is_pressed(params[(Param::NegateB + i)]) ? 10.F - b_input : b_input;

      auto const out = TEngine::compute(a, b);

      set_outputs(Output::And + i, Output::Nand + i, out.a_and_b_,
                  voltage_offset);
      set_outputs(Output::Or + i, Output::Nor + i, out.a_or_b_, voltage_offset);
      set_outputs(Output::Xor + i, Output::Xnor + i, out.a_xor_b_,
                  voltage_offset);
      set_outputs(Output::Implication + i, Output::Nonimplication + i,
                  out.a_implies_b_, voltage_offset);
      set_outputs(Output::ConverseImplication + i,
                  Output::ConverseNonimplication + i, out.b_implies_a_,
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
