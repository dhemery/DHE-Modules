#pragma once

#include "modules/controls/CommonInputs.h"

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
  auto level() const -> float { return rotation(this, LevelKnob, LevelCvInput, LevelAvKnob); }

  auto ccwLimit() const -> float {
    return scaledRotation<2>(this, CcwLimitKnob, CcwLimitCvInput, CcwLimitAvKnob, CcwLimitRangeSwitch, signalRanges);
  }

  auto cwLimit() const -> float {
    return scaledRotation<2>(this, CwLimitKnob, CwLimitCvInput, CwLimitAvKnob, CwLimitRangeSwitch, signalRanges);
  }
};

} // namespace dhe
