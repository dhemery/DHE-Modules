#pragma once

#include <functional>

#include "Module.h"

namespace dhe {

class Swave : public Module {
public:
  Swave();
  void process(const ProcessArgs &args) override;

  enum ParameterIds { CurveKnob, ShapeSwitch, ParameterCount };
  enum InputIds { CurveCv, MainIn, InputCount };
  enum OutputIds { MainOut, OutputCount };

private:
  void sendSignal(float voltage);
  auto signalIn() -> float;
  std::function<float(float)> taper;
};

} // namespace dhe
