#pragma once

#include "util/gain.h"
#include "util/signal.h"

#include <engine/Module.hpp>

#include <array>

namespace dhe {

class FuncChannel {
public:
  FuncChannel(rack::engine::Module *module, int inputIndex, int operandIndex,
              int outputIndex, int operatorSwitchIndex,
              int additionRangeSwitchIndex, int multiplicationRangeSwitchIndex);

  auto apply(float upstream) -> float;

  static const std::array<Range const *, 4> additionRanges;
  static const std::array<Range const *, 4> multiplicationRanges;

private:
  rack::engine::Input &input;
  rack::engine::Param &operand;
  rack::engine::Output &output;
  rack::engine::Param &operatorSwitch;
  rack::engine::Param &additionRangeSwitch;
  rack::engine::Param &multiplicationRangeSwitch;

  auto add(float in, float rotation) const -> float;
  auto multiply(float in, float rotation) const -> float;
};
} // namespace dhe
