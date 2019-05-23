#include "modules/Swave.h"

#include "modules/controls/Curvature.h"
#include "modules/params/CurveParams.h"

#include "util/range.h"
#include "util/sigmoid.h"
#include "util/signal.h"

namespace dhe {

Swave::Swave() {
  config(ParameterCount, InputCount, OutputCount);

  curvature::configKnob(this, CurveKnob);
  curvature::configSwitch(this, ShapeSwitch);

  taper = curvature::withSelectableShape(this, CurveKnob, CurveCv, ShapeSwitch);
}

void Swave::process(const ProcessArgs &args) {
  auto const normalized = Signal::bipolar_range.normalize(signalIn());
  auto const shaped = taper(normalized);
  auto const outputVoltage = Signal::bipolar_range.scale(shaped);
  sendSignal(outputVoltage);
}

void Swave::sendSignal(float voltage) { outputs[MainOut].setVoltage(voltage); }

auto Swave::signalIn() -> float { return inputs[MainIn].getVoltage(); }

} // namespace dhe
