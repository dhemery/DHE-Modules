#include "modules/Swave.h"

#include "util/range.h"
#include "util/sigmoid.h"
#include "util/signal.h"

namespace dhe {

Swave::Swave() {
  config(PARAMETER_COUNT, INPUT_COUNT, OUTPUT_COUNT);

  configParam(SHAPE_SWITCH, 0.f, 1.f, 0.f, "Curve Shape");
  configParam(CURVE_KNOB, 0.f, 1.f, 0.5f, "Curvature", "%", 0.f, 100.f, 0.f);

  shape = std::unique_ptr<CurvatureControl>(new CurvatureControl(
      params[CURVE_KNOB], params[SHAPE_SWITCH], inputs[CURVE_CV]));
}

void Swave::process(const ProcessArgs &args) {
  auto const normalized = Signal::bipolar_range.normalize(signalIn());
  auto const shaped = shape->taper(normalized);
  auto const outputVoltage = Signal::bipolar_range.scale(shaped);
  sendSignal(outputVoltage);
}

void Swave::sendSignal(float voltage) { outputs[MAIN_OUT].setVoltage(voltage); }

auto Swave::signalIn() -> float { return inputs[MAIN_IN].getVoltage(); }

} // namespace dhe
