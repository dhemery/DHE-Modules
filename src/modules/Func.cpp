#include <modules/Func.h>

namespace DHE {
Func::Func() : Module{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT} {}

void Func::step() { channel.adjust(0.f); }
} // namespace DHE
