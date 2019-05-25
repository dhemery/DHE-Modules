#include "modules/Func6.h"

#include "modules/controls/Controls.h"

#include <string>

namespace dhe {
Func6::Func6() {
  config(ParameterCount, InputCount, OutputCount);

  for (int i = 0; i < channelCount; i++) {
    auto const channelDescription = std::to_string(i + 1);
    knob::config(this, AmountKnob + i, "Amount " + channelDescription, "", {0.F, 1.F});
    toggle::config<2>(this, OperationSwitch + i, "Operation " + channelDescription,
                      {"Add (offset)", "Multiply (scale)"}, 0);
    toggle::config<4>(this, OffsetRangeSwitch + i, "Offset " + channelDescription + +" range",
                      {"0–5 V", "±5 V", "0–10 V", "±10 V"}, 1);
    toggle::config<4>(this, MultiplierRangeSwitch + i, "Multiplier " + channelDescription + " range",
                      {"0–1", "±1", "0–2", "±2"}, 2);

    channels.emplace_back(this, FuncInput + i, AmountKnob + i, FuncOutput + i, OperationSwitch + i,
                          OffsetRangeSwitch + i, MultiplierRangeSwitch + i);
  }
}

void Func6::process(const ProcessArgs & /*ignored*/) {
  auto upstream = 0.F;
  for (auto &channel : channels) {
    upstream = channel.apply(upstream);
  }
}

} // namespace dhe
