#pragma once

#include "conditions.h"
#include "controls/common-inputs.h"
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
      config_toggle<condition_count>(this, Param::Condition + i, "True when",
                                     condition_descriptions, 0);
    }
    for (int i = 0; i < pattern_count; i++) {
      config_toggle<outcome_count>(this, Param::Outcome + i, "Q",
                                   outcome_descriptions, 0);
    }
  }

  void process(ProcessArgs const & /*ignored*/) override {
    update_latches();
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
  void update_latches() {
    for (int i = 0; i < N; i++) {
      auto &latch = input_latches_[i];
      latch.clock(input_signal(i));
    }
  }

  auto outcome() -> bool { return outcome_for(selection()); }

  auto outcome_for(int i) const -> bool {
    return value_of(params[Param::Outcome + i]) > 0.5F;
  }

  auto selection() const -> int {
    auto pattern = 0;
    for (int i = 0; i < N; i++) {
      auto const satisfied = is_satisfied(condition(i), input_latches_[i]);
      pattern += pattern + (satisfied ? 1 : 0);
    }
    return pattern;
  }

  auto input_signal(int i) const -> bool {
    return is_high(inputs[Input::Input + i]) ||
           is_pressed(params[Param::InputOverride + i]);
  }

  auto condition(int i) const -> Condition {
    return static_cast<Condition>(value_of(params[Param::Condition + i]));
  }

  static auto constexpr pattern_count = 1 << N;
  using Param = ParamIds<N>;
  using Input = InputIds<N>;
  using Output = OutputIds;
  std::array<dhe::Latch, N> input_latches_{};
};
} // namespace truth
} // namespace dhe
