#pragma once

#include <array>
#include <engine/Module.hpp>
#include <functional>
#include <string>

namespace dhe {

class Range;

class FuncChannel {
public:
  FuncChannel() = default;
  FuncChannel(rack::engine::Module *module, int inputIndex, int operandIndex, int outputIndex, int operationSwitchIndex,
              int offsetRangeSwitchIndex, int multiplierRangeSwitchIndex, std::string const &channelName = "");

  auto apply(float upstream) -> float;

  auto currentOperandName() const -> std::string;

  auto currentOperandRange() const -> Range const *;

private:
  rack::engine::Input *input{};
  rack::engine::Param *operand{};
  rack::engine::Output *output{};

  rack::engine::Param *operationSwitch{};
  std::function<Range const *()> multiplierRange{};
  std::function<Range const *()> offsetRange{};
  std::string channelName;

  auto add(float in, float rotation) const -> float;
  auto multiply(float in, float rotation) const -> float;
  auto isMultiplication() const -> bool;
  std::string operatorName(int selection);
  std::string operandName(int selection);
};
} // namespace dhe
