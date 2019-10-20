#include "Upstage.h"

#include "CommonConfig.h"
#include "LevelConfig.h"

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
