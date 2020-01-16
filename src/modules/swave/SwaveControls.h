#pragma once

namespace dhe {
namespace swave {
  class SwaveControls {
  public:
    enum ParameterIds { CurveKnob, ShapeSwitch, CurveAvKnob, ParameterCount };
    enum InputIds { CurveCvInput, SwaveInput, InputCount };
    enum OutputIds { SwaveOutput, OutputCount };
  };
} // namespace swave
}; // namespace dhe