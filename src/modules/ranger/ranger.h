#pragma once

#include "components/cxmath.h"
#include "config/common-config.h"
#include "config/level-config.h"
#include "controls/common-inputs.h"
#include "controls/level-inputs.h"
#include "ranger-controls.h"
#include <engine/Module.hpp>

namespace dhe {
namespace ranger {

class Ranger : public rack::engine::Module {
  using Controls = RangerControls;

public:
  Ranger() {
    config(Controls::ParameterCount, Controls::InputCount,
           Controls::OutputCount);

    config_attenuator(this, Controls::LevelKnob, "Level");
    config_attenuverter(this, Controls::LevelAvKnob, "Level CV gain");

    config_level_knob(this, Controls::CcwLimitKnob,
                      Controls::CcwLimitRangeSwitch, "CCW limit");
    config_level_range_switch(this, Controls::CcwLimitRangeSwitch,
                              "CCW limit range", 0);
    config_attenuverter(this, Controls::CcwLimitAvKnob, "CCW limit CV gain");

    config_level_knob(this, Controls::CwLimitKnob, Controls::CwLimitRangeSwitch,
                      "CW limit");
    config_level_range_switch(this, Controls::CwLimitRangeSwitch,
                              "CW limit range", 0);
    config_attenuverter(this, Controls::CwLimitAvKnob, "CW limit CV gain");
  }

  void process(ProcessArgs const & /*args*/) override {
    auto const output_voltage = cx::scale(level(), ccw_limit(), cw_limit());
    outputs[Controls::RangerOutput].setVoltage(output_voltage);
  }

  auto dataToJson() -> json_t * override {
    auto *data = json_object();
    json_object_set_new(data, preset_version_key, json_integer(0));
    return data;
  }

private:
  auto level() const -> float {
    return rotation(params[Controls::LevelKnob], inputs[Controls::LevelCvInput],
                    params[Controls::LevelAvKnob]);
  }

  auto ccw_limit() const -> float {
    return selectable_level(params[Controls::CcwLimitKnob],
                            inputs[Controls::CcwLimitCvInput],
                            params[Controls::CcwLimitAvKnob],
                            params[Controls::CcwLimitRangeSwitch]);
  }

  auto cw_limit() const -> float {
    return selectable_level(
        params[Controls::CwLimitKnob], inputs[Controls::CwLimitCvInput],
        params[Controls::CwLimitAvKnob], params[Controls::CwLimitRangeSwitch]);
  }
};

} // namespace ranger
} // namespace dhe
