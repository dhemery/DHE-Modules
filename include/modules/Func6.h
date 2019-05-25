#pragma once

#include "Module.h"
#include "func/FuncChannel.h"

namespace dhe {
class Func6 : public Module {
  static constexpr auto channelCount = 6;

public:
  Func6();

  void process(const ProcessArgs &args) override;

  enum ParameterIds {
    OperandKnob,
    OperationSwitch = OperandKnob + channelCount,
    OffsetRangeSwitch = OperationSwitch + channelCount,
    MultiplierRangeSwitch = OffsetRangeSwitch + channelCount,
    ParameterCount = MultiplierRangeSwitch + channelCount
  };

  enum InputIds { FuncInput, InputCount = FuncInput + channelCount };

  enum OutputIds { FuncOutput, OutputCount = FuncOutput + channelCount };

  std::vector<FuncChannel> channels{};
};
} // namespace dhe
