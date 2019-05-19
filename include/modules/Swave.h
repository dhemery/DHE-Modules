#pragma once

#include "Module.h"
#include "modules/controls/CurvatureControl.h"

namespace DHE {

class Swave : public Module {
public:
  Swave();
  void process(const ProcessArgs &args) override;

  enum ParameterIds { CURVE_KNOB, SHAPE_SWITCH, PARAMETER_COUNT };
  enum InputIds { CURVE_CV, MAIN_IN, INPUT_COUNT };
  enum OutputIds { MAIN_OUT, OUTPUT_COUNT };

private:
  void sendSignal(float voltage);
  auto signalIn() -> float;
  std::unique_ptr<CurvatureControl> shape;
};

} // namespace DHE
