#include "modules/Ranger.h"

#include "config/CommonConfig.h"
#include "config/LevelConfig.h"
namespace dhe {

Ranger::Ranger() {
  config(ParameterCount, InputCount, OutputCount);

  configAttenuator(this, LevelKnob, "Level");
  configAttenuverter(this, LevelAvKnob, "Level CV gain");

  configLevelKnob(this, CcwLimitKnob, CcwLimitRangeSwitch, "CCW limit");
  configLevelRangeSwitch(this, CcwLimitRangeSwitch, "CCW limit range", 0);
  configAttenuverter(this, CcwLimitAvKnob, "CCW limit CV gain");

  configLevelKnob(this, CwLimitKnob, CwLimitRangeSwitch, "CW limit");
  configLevelRangeSwitch(this, CwLimitRangeSwitch, "CW limit range", 0);
  configAttenuverter(this, CwLimitAvKnob, "CW limit CV gain");
}

void Ranger::process(const ProcessArgs & /*args*/) {
  auto const outputVoltage = scale(level(), ccwLimit(), cwLimit());
  outputs[RangerOutput].setVoltage(outputVoltage);
}

} // namespace dhe
