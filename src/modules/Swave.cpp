#include "modules/Swave.h"

#include "util/range.h"
#include "util/sigmoid.h"
#include "util/signal.h"

namespace DHE {

Swave::Swave() : Module{PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT} {}

auto Swave::curve() const -> float {
  return Sigmoid::curvature(modulated(CURVE_KNOB, CURVE_CV));
}

auto Swave::is_s() const -> bool { return params[SHAPE_SWITCH].value > 0.5f; }

void Swave::send_signal(float voltage) { outputs[MAIN_OUT].value = voltage; }

auto Swave::signal_in() const -> float { return inputs[MAIN_IN].value; }

void Swave::process(const ProcessArgs &args) {
  auto phase = Signal::bipolar_range.normalize(signal_in());
  auto shaped = Sigmoid::taper(phase, curve(), is_s());
  auto out_voltage = Signal::bipolar_range.scale(shaped);
  send_signal(out_voltage);
}

} // namespace DHE
