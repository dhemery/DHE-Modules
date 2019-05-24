#include "modules/Ranger.h"

#include "modules/controls/Controls.h"
#include "modules/controls/Level.h"

namespace dhe {

Ranger::Ranger() {
  config(ParameterCount, InputCount, OutputCount);

  configKnob(LevelKnob, "Level");
  configCvGain(LevelAvKnob, "Level");

  level::configKnob(this, CcwLimitKnob, CcwLimitRangeSwitch, "CCW Limit", 0.F);
  level::configSwitch(this, CcwLimitRangeSwitch, "CCW Limit Range", 0);
  configCvGain(CcwLimitAvKnob, "CCW Limit");

  level::configKnob(this, CwLimitKnob, CwLimitRangeSwitch, "CW Limit", 1.F);
  level::configSwitch(this, CwLimitRangeSwitch, "CW Limit Range", 0);
  configCvGain(CwLimitAvKnob, "CW Limit");

  level = control::knob::rotation(this, LevelKnob, LevelCvInput, LevelAvKnob);
  ccwLimit = level::withSelectableRange(this, CcwLimitKnob, CcwLimitCvInput, CcwLimitAvKnob, CcwLimitRangeSwitch);
  cwLimit = level::withSelectableRange(this, CwLimitKnob, CwLimitCvInput, CwLimitAvKnob, CwLimitRangeSwitch);
}

void Ranger::process(const ProcessArgs & /*args*/) {
  auto const outputVoltage = scale(level(), ccwLimit(), cwLimit());
  outputs[RangerOutput].setVoltage(outputVoltage);
}

} // namespace dhe
