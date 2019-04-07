#include <modules/Swave.h>

#include "util/range.h"
#include "util/rotation.h"
#include "util/sigmoid.h"
#include "util/signal.h"

namespace DHE {

Swave::Swave() : Module{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT} {}

void Swave::step() {
  auto phase = Signal::bipolar_range.normalize(signal_in());
  auto shaped = Sigmoid::taper(phase, curve(), shape());
  auto out_voltage = Signal::bipolar_range.scale(shaped);
  send_signal(out_voltage);
}

auto Swave::curve() const -> float {
  auto rotation = params[CURVE_KNOB].value;
  auto cv = inputs[CURVE_CV].value;
  return Rotation::modulated(rotation, cv);
}

void Swave::send_signal(float voltage) { outputs[MAIN_OUT].value = voltage; }

auto Swave::shape() const -> float { return params[SHAPE_SWITCH].value; }

auto Swave::signal_in() const -> float { return inputs[MAIN_IN].value; }

} // namespace DHE
