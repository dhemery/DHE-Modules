#include "module/swave/Swave.h"

#include "config/CurvatureConfig.h"

namespace dhe {

Swave::Swave() {
  config(ParameterCount, InputCount, OutputCount);
  configCurvatureKnob(this, CurveKnob);
  configAttenuverter(this, CurveAvKnob, "Curvature CV gain");
  configCurveShapeSwitch(this, ShapeSwitch);
}

void Swave::process(const ProcessArgs & /*args*/) {
  auto const normalized = bipolarSignalRange.normalize(signalIn());
  auto const tapered = taper(normalized);
  auto const outputVoltage = bipolarSignalRange.scale(tapered);
  sendSignal(outputVoltage);
}
} // namespace dhe
