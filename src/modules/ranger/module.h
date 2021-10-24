#pragma once

#include "control-ids.h"

#include "components/cxmath.h"
#include "params/level-config.h"
#include "params/presets.h"
#include "signals/common-inputs.h"
#include "signals/level-inputs.h"

#include "rack.hpp"

namespace dhe {
namespace ranger {

struct Module : public rack::engine::Module {
  Module() {
    config(Param::Count, Input::Count, Output::Count);

    Attenuator::config(this, Param::Level, "Level");
    configInput(Input::LevelCv, "Level CV");
    Attenuverter::config(this, Param::LevelAv, "Level CV gain");

    config_level_knob(this, Param::CcwLimit, Param::CcwLimitRange, "CCW limit");
    config_level_range_switch(this, Param::CcwLimitRange, "CCW limit range", 0);
    configInput(Input::CcwLimitCv, "CCW limit CV");
    Attenuverter::config(this, Param::CcwLimitAv, "CCW limit CV gain");

    config_level_knob(this, Param::CwLimit, Param::CwLimitRange, "CW limit");
    config_level_range_switch(this, Param::CwLimitRange, "CW limit range", 0);
    configInput(Input::CwLimitCv, "CW limit CV");
    Attenuverter::config(this, Param::CwLimitAv, "CW limit CV gain");

    configOutput(Output::Main, "Ranger");
  }

  void process(ProcessArgs const & /*args*/) override {
    auto const output_voltage = cx::scale(level(), ccw_limit(), cw_limit());
    outputs[Output::Main].setVoltage(output_voltage);
  }

  auto dataToJson() -> json_t * override {
    auto *data = json_object();
    json_object_set_new(data, preset_version_key, json_integer(0));
    return data;
  }

private:
  auto level() const -> float {
    return rotation(params[Param::Level], inputs[Input::LevelCv],
                    params[Param::LevelAv]);
  }

  auto ccw_limit() const -> float {
    return selectable_level(params[Param::CcwLimit], inputs[Input::CcwLimitCv],
                            params[Param::CcwLimitAv],
                            params[Param::CcwLimitRange]);
  }

  auto cw_limit() const -> float {
    return selectable_level(params[Param::CwLimit], inputs[Input::CwLimitCv],
                            params[Param::CwLimitAv],
                            params[Param::CwLimitRange]);
  }
};

} // namespace ranger
} // namespace dhe
