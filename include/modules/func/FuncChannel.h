#pragma once

#include <array>

#include <engine/Module.hpp>

#include "modules/controls/Common.h"
#include "util/gain.h"
#include "util/signal.h"

namespace DHE {

class FuncChannel {
public:
  FuncChannel(rack::engine::Module *module, int inputIndex, int operandIndex,
              int outputIndex, int operatorSwitchIndex,
              int additionRangeSwitchIndex, int multiplicationRangeSwitchIndex);

  auto apply(float upstream) -> float;

  static const std::array<Range const *, 4> additionRanges;
  static const std::array<Range const *, 4> multiplicationRanges;

private:
  Input &input;
  Param &operand;
  Output &output;
  Param &operatorSwitch;
  Param &additionRangeSwitch;
  Param &multiplicationRangeSwitch;

  auto add(float in, float rotation) const -> float;
  auto multiply(float in, float rotation) const -> float;
};
} // namespace DHE
