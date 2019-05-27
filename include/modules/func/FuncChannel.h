#pragma once

#include <array>
#include <engine/Module.hpp>
#include <string>

namespace dhe {

class Range;

class FuncChannel {
public:
  FuncChannel() = default;
  FuncChannel(rack::engine::Module *module, int inputIndex, int amountIndex, int outputIndex, int operationSwitchIndex,
              int offsetRangeSwitchIndex, int multiplierRangeSwitchIndex, std::string const &channelDescription = "");

  auto apply(float upstream) -> float;

  static const std::array<Range const *, 4> offsetRanges;
  static const std::array<Range const *, 4> multiplierRanges;

private:
  rack::engine::Input *input{};
  rack::engine::Param *amount{};
  rack::engine::Output *output{};
  rack::engine::Param *operationSwitch{};
  rack::engine::Param *offsetRangeSwitch{};
  rack::engine::Param *multiplierRangeSwitch{};

  auto add(float in, float rotation) const -> float;
  auto multiply(float in, float rotation) const -> float;
};
} // namespace dhe
