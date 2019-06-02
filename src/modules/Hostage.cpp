#include "modules/Hostage.h"

#include "modules/controls/CommonConfig.h"
#include "modules/controls/DurationConfig.h"

namespace dhe {
Hostage::Hostage() {
  config(ParameterCount, InputCount, OutputCount);

  configDurationKnob(this, DurationKnob, DurationRangeSwitch);
  configDurationRangeSwitch(this, DurationRangeSwitch);

  configToggle<2>(this, ModeSwitch, "Mode", {"Hold", "Sustain"}, 0);

  stateMachine.start();
}

void Hostage::process(const ProcessArgs &args) { stateMachine.step(args.sampleTime); }

} // namespace dhe
