#pragma once

#include "CommonInputs.h"

#include <array>
#include <engine/Module.hpp>
#include <string>

namespace dhe {

class Range;

class FuncChannel {
public:
  FuncChannel(rack::engine::Module *module, int inputId, int operandKnobId, int outputId, int operationSwitchId,
              int offsetRangeSwitchId, int multiplierRangeSwitchId, std::string channelName = "");

  auto apply(float upstream) const -> float;

  void config();

  auto currentOperandName() const -> std::string;

  auto currentOperandRange() const -> const Range * { return isMultiplication() ? multiplierRange() : offsetRange(); }

private:
  static std::array<Range const *, 4> const multiplierRanges;
  static std::array<Range const *, 4> const offsetRanges;

  rack::engine::Module *module;
  std::string channelName;

  int const inputId{};
  int const multiplierRangeSwitchId{};
  int const offsetRangeSwitchId{};
  int const operandKnobId{};
  int const operationSwitchId{};
  int const outputId{};

  auto add(float in) const -> float { return in + offsetRange()->scale(operand()); }

  auto isMultiplication() const -> bool { return switchPosition(module->params[operationSwitchId]) == 1; }

  auto multiplierRange() const -> Range const * {
    return selectedRange<4>(module->params[multiplierRangeSwitchId], multiplierRanges);
  };

  auto multiply(float in) const -> float { return in * multiplierRange()->scale(operand()); }

  auto offsetRange() const -> Range const * {
    return selectedRange<4>(module->params[offsetRangeSwitchId], offsetRanges);
  };

  auto operand() const -> float { return paramValue(module->params[operandKnobId]); }
};
} // namespace dhe
