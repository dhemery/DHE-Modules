#pragma once

#include "./control-ids.h"
#include "./gate-modes.h"
#include "./input-selector.h"
#include "./outcomes.h"
#include "./params.h"
#include "./upgrader.h"
#include "signals/basic.h"

#include "rack.hpp"

#include <array>
#include <string>

namespace dhe {

namespace truth {
auto constexpr version = 1;

template <int N> class Module : public rack::engine::Module {
public:
  Module() {
    static auto const input_names =
        std::vector<std::string>{"A", "B", "C", "D"};

    config(Param::Count, Input::Count, Output::Count);

    for (int i = 0; i < N; i++) {
      auto input_name = input_names[i];
      if (i == N - 1) {
        input_name += "/Gate";
      }
      Button::config(this, Param::ForceInputHigh + i, input_name);
      configInput(Input::Input + i, input_name);
    }

    Switch::config<GateModes>(this, Param::GateMode, "True when",
                              GateMode::High);
    static auto constexpr rows = 1 << N;
    for (int row = 0; row < rows; row++) {
      Switch::config<Outcomes>(this, Param::Outcome + row, "Q", Outcome::True);
    }
    Button::config(this, Param::ForcQHigh, "Q");
    configOutput(Output::Q, "Q");
    Button::config(this, Param::ForceQNotHigh, "¬Q");
    configOutput(Output::QNot, "¬Q");
  }

  void process(ProcessArgs const & /*ignored*/) override {
    gate_.clock(is_true(N - 1));
    auto const q = outcome() ? 10.F : 0.F;
    outputs[Output::Q].setVoltage(q);
    outputs[Output::QNot].setVoltage(10.F - q);
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
    if (is_pressed(params[Param::ForcQHigh])) {
      return true;
    }
    if (is_pressed(params[Param::ForceQNotHigh])) {
      return false;
    }
    return outcome_for(selection());
  }

  auto outcome_for(int row) const -> bool {
    auto const outcome =
        static_cast<Outcome>(value_of(params[Param::Outcome + row]));
    switch (outcome) {
    default:
      return false;
    case Outcome::True:
      return true;
    case Outcome::Q:
      return is_high(outputs[Output::Q]);
    case Outcome::QNot:
      return is_high(outputs[Output::QNot]);
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
    return is_high(inputs[Input::Input + i]) ||
           is_pressed(params[Param::ForceInputHigh + i]);
  }

  auto gate_mode() const -> GateMode {
    return static_cast<GateMode>(value_of(params[Param::GateMode]));
  }

  using Param = ParamIds<N>;
  using Input = InputIds<N>;
  using Output = OutputIds;
  dhe::Latch gate_{};
};
} // namespace truth
} // namespace dhe
