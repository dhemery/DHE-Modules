#include "modules/Swave.h"

#include "modules/controls/CurvatureControls.h"
#include "modules/controls/Level.h"
#include "modules/controls/TaperControls.h"

namespace dhe {

Swave::Swave() {
  config(ParameterCount, InputCount, OutputCount);

  configCurvatureKnob(this, CurveKnob);
  configShapeSwitch(this, ShapeSwitch);

  taper = selectableShapeTaperFunction(this, CurveKnob, CurveCv, ShapeSwitch);
}

void Swave::process(const ProcessArgs & /*args*/) {
  auto const normalized = level::bipolarRange.normalize(signalIn());
  auto const tapered = taper(normalized);
  auto const outputVoltage = level::bipolarRange.scale(tapered);
  sendSignal(outputVoltage);
}

void Swave::sendSignal(float voltage) { outputs[SwaveOutput].setVoltage(voltage); }

auto Swave::signalIn() -> float { return inputs[SwaveInput].getVoltage(); }

} // namespace dhe
