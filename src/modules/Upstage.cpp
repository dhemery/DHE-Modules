#include "modules/Upstage.h"

#include "modules/controls/Controls.h"
#include "modules/controls/Level.h"

namespace dhe {

Upstage::Upstage() {
  config(ParameterCount, InputCount, OutputCount);

  level::configKnob(this, LevelKnob, LevelRangeSwitch);
  level::configSwitch(this, LevelRangeSwitch);

  configButton(this, TriggerButton, "TRIG", {"From input", "High"}, 0);
  configButton(this, WaitButton, "WAIT", {"From input", "High"}, 0);
}

void Upstage::process(const ProcessArgs & /*args*/) {
  auto isTriggered = triggerIn() && !waitIn();
  sendTrigger(isTriggered);
  sendEnvelope(level());
}

} // namespace dhe
