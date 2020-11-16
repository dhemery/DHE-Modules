#pragma once

#include "controls/common-inputs.h"
#include "gate-modes.h"
#include "input-selector.h"
#include "outcomes.h"
#include "truth-control-ids.h"

#include <engine/Module.hpp>
#include <string>

namespace dhe {

namespace truth {

template <int N> class Truth : public rack::engine::Module {
public:
  Truth() {
    config(Param::Count, Input::Count, Output::Count);
    for (int i = 0; i < N; i++) {
      config_button(this, Param::InputOverride + i, "Input",
                    {"From port", "High"}, 0);
    }
    config_toggle<input_0_selection_count>(this, Param::Input0Selector,
                                           "Column Input",
                                           input_0_selection_descriptions, 0);
    config_toggle<gate_mode_count>(this, Param::GateMode, "True when",
                                   gate_mode_descriptions, 3);
    for (int i = 0; i < pattern_count; i++) {
      config_toggle<outcome_count>(this, Param::Outcome + i, "Q",
                                   outcome_descriptions, 0);
    }
  }

  void process(ProcessArgs const & /*ignored*/) override {
    gate_.clock(is_true(N - 1));
    auto const q = outcome() ? 10.F : 0.F;
    outputs[Output::Q].setVoltage(q);
    outputs[Output::NotQ].setVoltage(10.F - q);
  }

  auto dataToJson() -> json_t * override {
    auto *data = json_object();
    json_object_set_new(data, preset_version_key, json_integer(0));
    return data;
  }

private:
  auto outcome() -> bool { return outcome_for(selection()); }

  auto outcome_for(int i) const -> bool {
    return value_of(params[Param::Outcome + i]) > 0.5F;
  }

  auto selection() const -> int {
    auto pattern = 0;
    if (input_0() == Input0Selection::Q) {
      pattern = is_high(outputs[Output::Q]);
    } else { // First input is A
      pattern = is_true(0) ? 1 : 0;
    }
    for (int i = 1; i < N - 1; i++) {
      pattern += pattern + (is_true(i) ? 1 : 0);
    }
    pattern += pattern + (is_satisfied(gate_mode(), gate_) ? 1 : 0);
    return pattern;
  }

  auto is_true(int i) const -> bool {
    return is_high(inputs[Input::Input + i]) ||
           is_pressed(params[Param::InputOverride + i]);
  }

  auto input_0() const -> Input0Selection {
    return static_cast<Input0Selection>(
        value_of(params[Param::Input0Selector]));
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
