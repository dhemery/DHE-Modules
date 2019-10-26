#include "module/func/Func6.h"

#include <string>

namespace dhe {
Func6::Func6() {
  config(ParameterCount, InputCount, OutputCount);

  for (int i = 0; i < channelCount; i++) {
    auto const channelDescription = std::string{" "} + std::to_string(i + 1);
    channels.emplace_back(this, FuncInput + i, AmountKnob + i, FuncOutput + i, OperationSwitch + i,
                          OffsetRangeSwitch + i, MultiplierRangeSwitch + i, channelDescription);
  }

  for (auto &channel : channels) {
    channel.config();
  }
}

void Func6::process(const ProcessArgs & /*ignored*/) {
  auto upstream = 0.F;
  for (auto const &channel : channels) {
    upstream = channel.apply(upstream);
  }
}

} // namespace dhe
