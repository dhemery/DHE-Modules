#pragma once

#include "modules/controls/Controls.h"
#include "modules/controls/Inputs.h"
#include "modules/controls/Level.h"

#include <engine/Module.hpp>
#include <functional>

namespace dhe {

class Ranger : public rack::engine::Module {
public:
  Ranger();

  void process(const ProcessArgs &args) override;

  enum ParameterIds {
    LevelKnob,
    LevelAvKnob,
    CcwLimitKnob,
    CcwLimitAvKnob,
    CcwLimitRangeSwitch,
    CwLimitKnob,
    CwLimitAvKnob,
    CwLimitRangeSwitch,
    ParameterCount
  };
  enum InputIds { LevelCvInput, CcwLimitCvInput, CwLimitCvInput, InputCount };
  enum OutputIds { RangerOutput, OutputCount };

private:
  auto level() -> float { return rotation(this, LevelKnob, LevelCvInput, LevelAvKnob); }

  auto ccwLimit() -> float {
    return scaledRotation<2>(this, CcwLimitKnob, CcwLimitCvInput, CcwLimitAvKnob, CcwLimitRangeSwitch, level::ranges);
  }

  auto cwLimit() -> float {
    return scaledRotation<2>(this, CwLimitKnob, CwLimitCvInput, CwLimitAvKnob, CwLimitRangeSwitch, level::ranges);
  }
};

} // namespace dhe
