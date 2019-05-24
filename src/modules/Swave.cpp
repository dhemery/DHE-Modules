#include "modules/Swave.h"

#include "modules/controls/Curvature.h"
#include "modules/controls/Level.h"
#include "util/range.h"
#include "util/sigmoid.h"

namespace dhe {

Swave::Swave() {
  config(ParameterCount, InputCount, OutputCount);

  curvature::configKnob(this, CurveKnob);
  curvature::configSwitch(this, ShapeSwitch);

  taper = curvature::withSelectableShape(this, CurveKnob, CurveCv, ShapeSwitch);
}

void Swave::process(const ProcessArgs &args) {
  auto const normalized = level::bipolarRange.normalize(signalIn());
  auto const shaped = taper(normalized);
  auto const outputVoltage = level::bipolarRange.scale(shaped);
  sendSignal(outputVoltage);
}

void Swave::sendSignal(float voltage) { outputs[SwaveOutput].setVoltage(voltage); }

auto Swave::signalIn() -> float { return inputs[SwaveInput].getVoltage(); }

} // namespace dhe
