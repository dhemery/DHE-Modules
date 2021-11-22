#pragma once

#include "control-ids.h"
#include "controls/buttons.h"
#include "controls/switches.h"
#include "gate-mode.h"
#include "input-selector.h"
#include "outcome.h"
#include "params/presets.h"
#include "signals/basic.h"
#include "upgrader.h"

#include "rack.hpp"

#include <string>
#include <vector>

namespace dhe {

namespace truth {
auto constexpr version = 1;

template <int N> struct Module : rack::engine::Module {
  Module() {
    static auto const input_names =
        std::vector<std::string>{"A", "B", "C", "D"};

    config(ParamId::Count, InputId::Count, OutputId::Count);

    for (int i = 0; i < N; i++) {
      auto input_name = input_names[i];
      if (i == N - 1) {
        input_name += "/Gate";
      }
      Button::config(this, ParamId::ForceInputHigh + i, input_name);
      configInput(InputId::Input + i, input_name);
    }

    Switch::config<GateModes>(this, ParamId::GateMode, "True when",
                              GateMode::High);
    static auto constexpr rows = 1 << N;
    for (int row = 0; row < rows; row++) {
      Switch::config<Outcomes>(this, ParamId::Outcome + row, "Q",
                               Outcome::True);
    }
    Button::config(this, ParamId::ForcQHigh, "Q");
    configOutput(OutputId::Q, "Q");
    Button::config(this, ParamId::ForceQNotHigh, "¬Q");
    configOutput(OutputId::QNot, "¬Q");
  }

  void process(ProcessArgs const & /*ignored*/) override {
    gate_.clock(is_true(N - 1));
    auto const q = outcome() ? 10.F : 0.F;
    outputs[OutputId::Q].setVoltage(q);
    outputs[OutputId::QNot].setVoltage(10.F - q);
  }

  void dataFromJson(json_t *data) override {
    auto *const preset_version_json = json_object_get(data, preset_version_key);
    auto const preset_version = json_integer_value(preset_version_json);
    if (preset_version != version) {
      upgrade::from_v0<N>(params);
    }
  }

  auto dataToJson() -> json_t * override {
    auto *data = json_object();
    json_object_set_new(data, preset_version_key, json_integer(1));
    return data;
  }

private:
  auto outcome() -> bool {
    if (is_pressed(params[ParamId::ForcQHigh])) {
      return true;
    }
    if (is_pressed(params[ParamId::ForceQNotHigh])) {
      return false;
    }
    return outcome_for(selection());
  }

  auto outcome_for(int row) const -> bool {
    auto const outcome =
        static_cast<Outcome>(value_of(params[ParamId::Outcome + row]));
    switch (outcome) {
    default:
      return false;
    case Outcome::True:
      return true;
    case Outcome::Q:
      return is_high(outputs[OutputId::Q]);
    case Outcome::QNot:
      return is_high(outputs[OutputId::QNot]);
    }
  }

  auto selection() const -> int {
    auto input_state = 0;
    for (int i = 0; i < N - 1; i++) {
      input_state += input_state + (is_true(i) ? 1 : 0);
    }
    input_state += input_state + (is_satisfied(gate_mode(), gate_) ? 1 : 0);
    return input_state;
  }

  auto is_true(int i) const -> bool {
    return is_high(inputs[InputId::Input + i]) ||
           is_pressed(params[ParamId::ForceInputHigh + i]);
  }

  auto gate_mode() const -> GateMode {
    return static_cast<GateMode>(value_of(params[ParamId::GateMode]));
  }

  using ParamId = ParamIds<N>;
  using InputId = InputIds<N>;
  dhe::Latch gate_{};
};
} // namespace truth
} // namespace dhe
