#pragma once

#include "./control-ids.h"
#include "components/range.h"
#include "config/common-config.h"
#include "signals/common-inputs.h"

#include "rack.hpp"

namespace dhe {
namespace buttons {

class Module : public rack::engine::Module {
  using Param = ParamIds;
  using Output = OutputIds;

public:
  Module() {
    config(Param::Count, 0, Output::Count);
    auto const row_names =
        std::vector<std::string>{"A", "B", "C", "D", "E", "F", "G", "H"};
    for (int i = 0; i < button_count; i++) {
      configButton(Param::Button + i, "Switch " + row_names[i]);
      configButton(Param::Negate + i, "Negate " + row_names[i]);
      configOutput(Output::Out + i, row_names[i]);
    }
  }

  void process(ProcessArgs const & /*args*/) override {
    for (int i = 0; i < button_count; i++) {
      auto const button_is_pressed = is_pressed(params[Param::Button + i]);
      auto const negate_is_pressed = is_pressed(params[Param::Negate + i]);
      auto const out = button_is_pressed == negate_is_pressed ? 0.F : 10.F;
      outputs[Output::Out + i].setVoltage(out);
    }
  }

  auto dataToJson() -> json_t * override {
    auto *data = json_object();
    json_object_set_new(data, preset_version_key, json_integer(0));
    return data;
  }

private:
};
} // namespace buttons
} // namespace dhe
