#include "modules/Hostage.h"

#include "config/CommonConfig.h"
#include "config/DurationConfig.h"

namespace dhe {
Hostage::Hostage() {
  config(ParameterCount, InputCount, OutputCount);

  configDurationKnob(this, DurationKnob, DurationRangeSwitch);
  configDurationRangeSwitch(this, DurationRangeSwitch);

  configToggle<2>(this, ModeSwitch, "Mode", {"Hold", "Sustain"}, 0);
}

} // namespace dhe
