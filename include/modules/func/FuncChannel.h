#pragma once

#include "engine/Module.hpp"

#include "util/gain.h"
#include "util/signal.h"

namespace DHE {

enum FuncOperator { ADD, MULTIPLY };

class FuncChannel {
public:
  FuncChannel(rack::engine::Module *module, int inputIndex, int operandIndex,
              int outputIndex, int operatorSwitchIndex,
              int additionRangeSwitchIndex, int multiplicationRangeSwitchIndex,
              std::function<void(FuncOperator)> onOperatorChange);

  auto apply(float upstream) -> float;

private:
  rack::engine::Input &input;
  rack::engine::Param &operand;
  rack::engine::Output &output;
  rack::engine::Param &operatorSwitch;
  rack::engine::Param &additionRangeSwitch;
  rack::engine::Param &multiplicationRangeSwitch;

  FuncOperator op = ADD;

  auto add(float in, float rotation) const -> float;
  auto multiply(float in, float rotation) const -> float;
  void setOperator();

  std::function<void(FuncOperator)> onOperatorChange;
};
} // namespace DHE
