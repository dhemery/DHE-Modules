#pragma once

#include "modules/controls/Common.h"

namespace DHE {

class ModulatedKnob {
public:
  ModulatedKnob(Param &knob, Input &cvInput);
  auto rotation() -> float;

private:
  Param &knob;
  Input &cvInput;
};

} // namespace DHE
