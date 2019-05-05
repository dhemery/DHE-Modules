#include "modules/Swave.h"

#include "util/range.h"
#include "util/sigmoid.h"
#include "util/signal.h"

namespace DHE {

Swave::Swave() {
  config(PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT);

  configParam(SHAPE_SWITCH, 0.f, 1.f, 0.f, "Curve Shape");
  configParam(CURVE_KNOB, 0.f, 1.f, 0.5f, "Curvature", "%", 0.f, 100.f, 0.f);
}

auto Swave::curve() -> float {
  return Sigmoid::curvature(modulated(CURVE_KNOB, CURVE_CV));
}

auto Swave::is_s() -> bool { return params[SHAPE_SWITCH].getValue() > 0.5f; }

void Swave::send_signal(float voltage) {
  outputs[MAIN_OUT].setVoltage(voltage);
}

auto Swave::signal_in() -> float { return inputs[MAIN_IN].getVoltage(); }

void Swave::process(const ProcessArgs &args) {
  auto phase = Signal::bipolar_range.normalize(signal_in());
  auto shaped = Sigmoid::taper(phase, curve(), is_s());
  auto out_voltage = Signal::bipolar_range.scale(shaped);
  send_signal(out_voltage);
}

} // namespace DHE
