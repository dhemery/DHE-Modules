#include <modules/Func6.h>

namespace DHE {
Func6::Func6() : Module{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT} {
  for (int i = 0; i < channel_count; i++) {
    channels.emplace_back(this, IN + i, KNOB + i, OUT + i);
  }
}
void Func6::step() {
  auto upstream = 0.f;
  for (auto &channel : channels) {
    upstream = channel.adjust(upstream);
  }
}
} // namespace DHE
