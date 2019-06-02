#include "modules/Ranger.h"

#include "modules/controls/Controls.h"
#include "modules/controls/Level.h"

namespace dhe {

Ranger::Ranger() {
  config(ParameterCount, InputCount, OutputCount);

  configAttenuator(this, LevelKnob, "Level");
  configAttenuverter(this, LevelAvKnob, "Level CV gain");

  level::configKnob(this, CcwLimitKnob, CcwLimitRangeSwitch, "CCW limit", 0.F);
  level::configSwitch(this, CcwLimitRangeSwitch, "CCW limit range", 0);
  configAttenuverter(this, CcwLimitAvKnob, "CCW limit CV gain");

  level::configKnob(this, CwLimitKnob, CwLimitRangeSwitch, "CW limit", 1.F);
  level::configSwitch(this, CwLimitRangeSwitch, "CW limit range", 0);
  configAttenuverter(this, CwLimitAvKnob, "CW limit CV gain");
}

void Ranger::process(const ProcessArgs & /*args*/) {
  auto const outputVoltage = scale(level(), ccwLimit(), cwLimit());
  outputs[RangerOutput].setVoltage(outputVoltage);
}

} // namespace dhe
