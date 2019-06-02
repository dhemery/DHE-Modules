#include "modules/Hostage.h"

#include "modules/controls/Controls.h"
#include "modules/controls/Duration.h"

namespace dhe {
Hostage::Hostage() {
  config(ParameterCount, InputCount, OutputCount);

  duration::configKnob(this, DurationKnob, DurationRangeSwitch);
  duration::configSwitch(this, DurationRangeSwitch);

  configToggle<2>(this, ModeSwitch, "Mode", {"Hold", "Sustain"}, 0);

  stateMachine.start();
}

void Hostage::process(const ProcessArgs &args) { stateMachine.step(args.sampleTime); }

} // namespace dhe
