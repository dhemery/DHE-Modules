#pragma once

#include <array>

#include "ModulatedKnob.h"

#include "util/range.h"

namespace DHE {

class CurvatureControl : public ModulatedKnob {
  static ConstantParam constantJShapeSwitch;

public:
  explicit CurvatureControl(Param &knob,
                            Param &shapeSwitch = constantJShapeSwitch,
                            Input &cvInput = constant0VoltageInput,
                            Param &avParam = constantFullyRotatedKnobParam)
      : ModulatedKnob{knob, cvInput, avParam}, shapeSwitch{shapeSwitch} {};

  auto taper(float phase) -> float;

private:
  Param &shapeSwitch;
};

} // namespace DHE
