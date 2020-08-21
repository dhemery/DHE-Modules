#pragma once

#include "RangerControls.h"
#include "config/CommonConfig.h"
#include "config/LevelConfig.h"
#include "controls/CommonInputs.h"
#include "controls/LevelInputs.h"

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

    configLevelKnob(this, Controls::CcwLimitKnob, Controls::CcwLimitRangeSwitch,
                    "CCW limit");
    configLevelRangeSwitch(this, Controls::CcwLimitRangeSwitch,
                           "CCW limit range", 0);
    config_attenuverter(this, Controls::CcwLimitAvKnob, "CCW limit CV gain");

    configLevelKnob(this, Controls::CwLimitKnob, Controls::CwLimitRangeSwitch,
                    "CW limit");
    configLevelRangeSwitch(this, Controls::CwLimitRangeSwitch, "CW limit range",
                           0);
    config_attenuverter(this, Controls::CwLimitAvKnob, "CW limit CV gain");
  }

  void process(ProcessArgs const & /*args*/) override {
    auto const outputVoltage = scale(level(), ccwLimit(), cwLimit());
    outputs[Controls::RangerOutput].setVoltage(outputVoltage);
  }

private:
  auto level() const -> float {
    return rotation(params[Controls::LevelKnob], inputs[Controls::LevelCvInput],
                    params[Controls::LevelAvKnob]);
  }

  auto ccwLimit() const -> float {
    return selectableLevel(params[Controls::CcwLimitKnob],
                           inputs[Controls::CcwLimitCvInput],
                           params[Controls::CcwLimitAvKnob],
                           params[Controls::CcwLimitRangeSwitch]);
  }

  auto cwLimit() const -> float {
    return selectableLevel(
        params[Controls::CwLimitKnob], inputs[Controls::CwLimitCvInput],
        params[Controls::CwLimitAvKnob], params[Controls::CwLimitRangeSwitch]);
  }
};

} // namespace ranger
} // namespace dhe
