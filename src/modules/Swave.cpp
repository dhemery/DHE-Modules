#include "modules/Swave.h"

#include "modules/controls/CurvatureConfig.h"
#include "modules/controls/LevelConfig.h"

namespace dhe {

Swave::Swave() {
  config(ParameterCount, InputCount, OutputCount);
  configCurvatureKnob(this, CurveKnob);
  configCurveShapeSwitch(this, ShapeSwitch);
}

void Swave::process(const ProcessArgs & /*args*/) {
  auto const normalized = bipolarSignalRange.normalize(signalIn());
  auto const tapered = taper(normalized);
  auto const outputVoltage = bipolarSignalRange.scale(tapered);
  sendSignal(outputVoltage);
}
} // namespace dhe
