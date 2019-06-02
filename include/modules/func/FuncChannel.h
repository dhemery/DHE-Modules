#pragma once

#include "modules/controls/Inputs.h"

#include <array>
#include <engine/Module.hpp>
#include <string>

namespace dhe {

class Range;

class FuncChannel {
public:
  FuncChannel() = default;
  FuncChannel(rack::engine::Module *module, int inputId, int operandKnobId, int outputId, int operationSwitchId,
              int offsetRangeSwitchId, int multiplierRangeSwitchId, std::string const &channelName = "");

  auto apply(float upstream) -> float;

  auto currentOperandName() const -> std::string;

  auto currentOperandRange() const -> Range const *;

private:
  static const std::array<Range const *, 4> multiplierRanges;
  static const std::array<Range const *, 4> offsetRanges;

  rack::engine::Module const *module;
  std::string channelName;

  int const inputId{};
  int const multiplierRangeSwitchId{};
  int const offsetRangeSwitchId{};
  int const operandKnobId{};
  int const operationSwitchId{};
  int const outputId{};

  auto multiplierRange() const -> Range const * {
    return selectedRange<4>(module, multiplierRangeSwitchId, multiplierRanges);
  };

  auto offsetRange() const -> Range const * { return selectedRange<4>(module, offsetRangeSwitchId, offsetRanges); };

  auto add(float in, float rotation) const -> float;
  auto multiply(float in, float rotation) const -> float;
  auto isMultiplication() const -> bool;
};
} // namespace dhe
