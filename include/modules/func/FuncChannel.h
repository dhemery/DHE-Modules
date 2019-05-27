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
              int offsetRangeSwitchIndex, int multiplierRangeSwitchIndex, std::string const &channelDescription = "");

  auto apply(float upstream) -> float;

private:
  rack::engine::Input *input{};
  rack::engine::Param *operand{};
  rack::engine::Output *output{};
  rack::engine::Param *operationSwitch{};

  std::function<Range const *()> multiplierRange{};
  std::function<Range const *()> offsetRange{};
  auto add(float in, float rotation) const -> float;
  auto multiply(float in, float rotation) const -> float;
};
} // namespace dhe
