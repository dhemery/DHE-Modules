#include "modules/Ranger.h"

#include "modules/controls/Controls.h"
#include "modules/controls/Level.h"

namespace dhe {

Ranger::Ranger() {
  config(ParameterCount, InputCount, OutputCount);

  configKnob(LevelKnob, "Level");
  configCvGain(LevelAv, "Level");

  level::configKnob(this, CcwLimitKnob, CcwLimitRangeSwitch, "CCW Limit", 0.f);
  level::configSwitch(this, CcwLimitRangeSwitch, "CCW Limit Range", 0);
  configCvGain(CcwLimitAv, "CCW Limit");

  level::configKnob(this, CwLimitKnob, CwLimitRangeSwitch, "CW Limit", 1.f);
  level::configSwitch(this, CwLimitRangeSwitch, "CW Limit Range", 0);
  configCvGain(CwLimitAv, "CW Limit");

  level = control::knob::rotation(this, LevelKnob, LevelCv, LevelAv);
  ccwLimit = level::withSelectableRange(this, CcwLimitKnob, CcwLimitCv,
                                        CcwLimitAv, CcwLimitRangeSwitch);
  cwLimit = level::withSelectableRange(this, CwLimitKnob, CwLimitCv, CwLimitAv,
                                       CwLimitRangeSwitch);
}

void Ranger::process(const ProcessArgs &args) {
  auto const outputVoltage = scale(level(), ccwLimit(), cwLimit());
  outputs[MainOut].setVoltage(outputVoltage);
}

} // namespace dhe
