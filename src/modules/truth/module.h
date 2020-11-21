#pragma once

#include "control-ids.h"
#include "controls/common-inputs.h"
#include "gate-modes.h"
#include "input-selector.h"
#include "outcomes.h"
#include "upgrader.h"

#include <engine/Module.hpp>
#include <string>

namespace dhe {

namespace truth {
auto constexpr version = 1;

template <int N> class Truth : public rack::engine::Module {
public:
  Truth() {
    config(Param::Count, Input::Count, Output::Count);
    for (int i = 0; i < N; i++) {
      config_button(this, Param::InputOverride + i, "Input",
                    {"From port", "High"}, 0);
    }
    config_toggle<gate_mode_count>(this, Param::GateMode, "True when",
                                   gate_mode_descriptions, 3);
    static auto constexpr rows = 1 << N;
    for (int row = 0; row < rows; row++) {
      config_toggle<outcome_count>(this, Param::Outcome + row, "Q",
                                   outcome_descriptions, 0);
    }
    config_button(this, Param::QOverride, "Q", {"From table", "Set"}, 0);
    config_button(this, Param::QNotOverride, "¬Q", {"From table", "Set"}, 0);
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
    if (is_pressed(params[Param::QOverride])) {
      return true;
    }
    if (is_pressed(params[Param::QNotOverride])) {
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
           is_pressed(params[Param::InputOverride + i]);
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
