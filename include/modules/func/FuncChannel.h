#pragma once

#include "modules/controls/CommonInputs.h"

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
  static const std::array<Range const *, 4> multiplierRanges;
  static const std::array<Range const *, 4> offsetRanges;

  rack::engine::Module *module;
  std::string channelName;

  int const inputId{};
  int const multiplierRangeSwitchId{};
  int const offsetRangeSwitchId{};
  int const operandKnobId{};
  int const operationSwitchId{};
  int const outputId{};

  auto add(float in) const -> float { return in + offsetRange()->scale(operand()); }

  auto isMultiplication() const -> bool { return switchPosition(module, operationSwitchId) == 1; }

  auto multiplierRange() const -> Range const * {
    return selectedRange<4>(module, multiplierRangeSwitchId, multiplierRanges);
  };

  auto multiply(float in) const -> float { return in * multiplierRange()->scale(operand()); }

  auto offsetRange() const -> Range const * { return selectedRange<4>(module, offsetRangeSwitchId, offsetRanges); };

  auto operand() const -> float { return rotation(module, operandKnobId); }
};
} // namespace dhe
