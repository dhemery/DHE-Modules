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
    for (int i = 0; i < pattern_count; i++) {
      config_toggle<outcome_count>(this, Param::Outcome + i, "Q",
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
  auto outcome() -> bool { return outcome_for(selection()); }

  auto outcome_for(int i) const -> bool {
    auto const selection =
        static_cast<Outcome>(value_of(params[Param::Outcome + i]));
    switch (selection) {
    default:
      return false;
    case Outcome::True:
      return true;
    case Outcome::Q:
      return voltage_at(outputs[Output::Q]) > 0.5F;
    case Outcome::QNot:
      return voltage_at(outputs[Output::QNot]) > 0.5F;
    }
  }

  auto selection() const -> int {
    auto pattern = 0;
    for (int i = 0; i < N - 1; i++) {
      pattern += pattern + (is_true(i) ? 1 : 0);
    }
    pattern += pattern + (is_satisfied(gate_mode(), gate_) ? 1 : 0);
    return pattern;
  }

  auto is_true(int i) const -> bool {
    return is_high(inputs[Input::Input + i]) ||
           is_pressed(params[Param::InputOverride + i]);
  }

  auto gate_mode() const -> GateMode {
    return static_cast<GateMode>(value_of(params[Param::GateMode]));
  }

  static auto constexpr pattern_count = 1 << N;
  using Param = ParamIds<N>;
  using Input = InputIds<N>;
  using Output = OutputIds;
  dhe::Latch gate_{};
};
} // namespace truth
} // namespace dhe
