#include "modules/Ranger.h"

#include "modules/controls/Controls.h"
#include "modules/controls/Level.h"

namespace dhe {

Ranger::Ranger() {
  config(ParameterCount, InputCount, OutputCount);

  configParam(LevelKnob, 0.F, 1.F, 0.5F, "Level", "%", 0.F, 100.F, 0.F);
  attenuverter::config(this, LevelAvKnob, "Level CV gain");

  level::configKnob(this, CcwLimitKnob, CcwLimitRangeSwitch, "CCW limit", 0.F);
  level::configSwitch(this, CcwLimitRangeSwitch, "CCW limit range", 0);
  attenuverter::config(this, CcwLimitAvKnob, "CCW limit CV gain");

  level::configKnob(this, CwLimitKnob, CwLimitRangeSwitch, "CW limit", 1.F);
  level::configSwitch(this, CwLimitRangeSwitch, "CW limit range", 0);
  attenuverter::config(this, CwLimitAvKnob, "CW limit CV gain");

  level = control::knob::rotation(this, LevelKnob, LevelCvInput, LevelAvKnob);
  ccwLimit = level::withSelectableRange(this, CcwLimitKnob, CcwLimitCvInput, CcwLimitAvKnob, CcwLimitRangeSwitch);
  cwLimit = level::withSelectableRange(this, CwLimitKnob, CwLimitCvInput, CwLimitAvKnob, CwLimitRangeSwitch);
}

void Ranger::process(const ProcessArgs & /*args*/) {
  auto const outputVoltage = scale(level(), ccwLimit(), cwLimit());
  outputs[RangerOutput].setVoltage(outputVoltage);
}

} // namespace dhe
