#pragma once

#include "controls/common-inputs.h"
#include "steppers.h"
#include "truth-control-ids.h"

#include <engine/Module.hpp>
#include <string>

namespace dhe {

namespace truth {

class Truth3 : public rack::engine::Module {

public:
  Truth3() {
    config(Param::Count, Input::Count, Output::Count);
    for (int i = 0; i < input_count; i++) {
      config_button(this, Param::InputOverride + i, "Input",
                    {"From port", "High"}, 0);
      config_toggle<condition_count>(this, Param::Condition + i,
                                     "True when", condition_descriptions,
                                     0);
    }
    for (int i = 0; i < pattern_count; i++) {
      config_toggle<outcome_count>(this, Param::Outcome + i, "Q",
                                   outcome_descriptions, 0);
    }
  }

  void process(ProcessArgs const & /*ignored*/) override {}

  auto dataToJson() -> json_t * override {
    auto *data = json_object();
    json_object_set_new(data, preset_version_key, json_integer(0));
    return data;
  }

private:
  static auto constexpr input_names = "ABC";
  static auto constexpr input_count = 3;
  static auto constexpr pattern_count = 1 << input_count;
  using Param = ParamIds<input_count>;
  using Input = InputIds<input_count>;
  using Output = OutputIds;
};

} // namespace truth
} // namespace dhe
