#pragma once

#include "modules/controls/Common.h"

namespace DHE {

class ModulatedKnob {
public:
  ModulatedKnob(Param &knob, Input &cvInput, Param &avParam)
      : knob{knob}, cvInput{cvInput}, avParam{avParam} {}

  auto rotation() -> float;

private:
  Param &knob;
  Input &cvInput;
  Param &avParam;
};

} // namespace DHE
