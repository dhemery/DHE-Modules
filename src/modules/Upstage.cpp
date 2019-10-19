#include "modules/Upstage.h"

#include "config/CommonConfig.h"
#include "config/LevelConfig.h"

namespace dhe {

Upstage::Upstage() {
  config(ParameterCount, InputCount, OutputCount);

  configLevelKnob(this, LevelKnob, LevelRangeSwitch);
  configLevelRangeSwitch(this, LevelRangeSwitch);

  configButton(this, TriggerButton, "TRIG", {"From input", "High"}, 0);
  configButton(this, WaitButton, "WAIT", {"From input", "High"}, 0);
}

void Upstage::process(const ProcessArgs & /*args*/) {
  auto isTriggered = triggerIn() && !waitIn();
  sendTrigger(isTriggered);
  sendEnvelope(level());
}

} // namespace dhe
