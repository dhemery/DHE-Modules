#pragma once

#include "engine/Module.hpp"

#include "util/gain.h"
#include "util/signal.h"

namespace DHE {

static constexpr auto attenuation_range = Range{0.f, 1.f};
static constexpr auto invertible_attenuation_range = Range{-1.f, 1.f};
static constexpr auto invertible_gain_range = Range{-2.f, 2.f};

static constexpr auto half_bipolar_range = Range{0.f, 5.f};
static constexpr auto invertible_unipolar_range = Range{-10.f, 10.f};

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
