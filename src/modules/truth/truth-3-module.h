#pragma once

#include "controls/common-inputs.h"
#include "truth-control-ids.h"

#include <engine/Module.hpp>
#include <string>

namespace dhe {

namespace truth {

class Truth3 : public rack::engine::Module {

public:
  Truth3() { config(Param::Count, Input::Count, Output::Count); }

  void process(ProcessArgs const & /*ignored*/) override {

  }

  auto dataToJson() -> json_t * override {
    auto *data = json_object();
    json_object_set_new(data, preset_version_key, json_integer(0));
    return data;
  }

private:
  static auto constexpr input_count = 3;
  using Param = ParamIds<input_count>;
  using Input = InputIds<input_count>;
  using Output = OutputIds;
};

} // namespace truth
} // namespace dhe
