#pragma once

#include "Module.h"
#include "modules/controls/Controls.h"
#include "modules/controls/Level.h"

#include <functional>

namespace dhe {

class Ranger : public Module {
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
  std::function<float()> ccwLimit{
      level::withSelectableRange(this, CcwLimitKnob, CcwLimitCvInput, CcwLimitAvKnob, CcwLimitRangeSwitch)};
  std::function<float()> cwLimit{
      level::withSelectableRange(this, CwLimitKnob, CwLimitCvInput, CwLimitAvKnob, CwLimitRangeSwitch)};
  std::function<float()> level{knob::rotation(this, LevelKnob, LevelCvInput, LevelAvKnob)};
};

} // namespace dhe
