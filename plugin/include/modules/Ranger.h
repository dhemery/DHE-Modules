#pragma once

#include "modules/controls/CommonInputs.h"
#include "modules/controls/LevelInputs.h"

#include <engine/Module.hpp>

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
  auto level() const -> float { return rotation(params[LevelKnob], inputs[LevelCvInput], params[LevelAvKnob]); }

  auto ccwLimit() const -> float {
    return selectableLevel(params[CcwLimitKnob], inputs[CcwLimitCvInput], params[CcwLimitAvKnob],
                           params[CcwLimitRangeSwitch]);
  }

  auto cwLimit() const -> float {
    return selectableLevel(params[CwLimitKnob], inputs[CwLimitCvInput], params[CwLimitAvKnob],
                           params[CwLimitRangeSwitch]);
  }
};

} // namespace dhe
