#pragma once

#include "func/FuncChannel.h"

#include <engine/Module.hpp>
#include <vector>

namespace dhe {
class Func6 : public rack::engine::Module {
  static constexpr auto channelCount = 6;

public:
  Func6();

  void process(const ProcessArgs &args) override;

  enum ParameterIds {
    ENUMS(AmountKnob, channelCount),
    ENUMS(OperationSwitch, channelCount),
    ENUMS(OffsetRangeSwitch, channelCount),
    ENUMS(MultiplierRangeSwitch, channelCount),
    ParameterCount
  };

  enum InputIds { FuncInput, InputCount = FuncInput + channelCount };

  enum OutputIds { FuncOutput, OutputCount = FuncOutput + channelCount };

private:
  std::vector<FuncChannel> channels;
};
} // namespace dhe
