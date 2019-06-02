#pragma once

#include <engine/Module.hpp>
#include <functional>

namespace dhe {

class Swave : public rack::engine::Module {
public:
  Swave();
  void process(const ProcessArgs &args) override;

  enum ParameterIds { CurveKnob, ShapeSwitch, ParameterCount };
  enum InputIds { CurveCv, SwaveInput, InputCount };
  enum OutputIds { SwaveOutput, OutputCount };

private:
  void sendSignal(float voltage);
  auto signalIn() -> float;
  std::function<float(float)> taper;
};

} // namespace dhe
