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
    AmountKnob,
    OperationSwitch = AmountKnob + channelCount,
    OffsetRangeSwitch = OperationSwitch + channelCount,
    MultiplierRangeSwitch = OffsetRangeSwitch + channelCount,
    ParameterCount = MultiplierRangeSwitch + channelCount
  };

  enum InputIds { FuncInput, InputCount = FuncInput + channelCount };

  enum OutputIds { FuncOutput, OutputCount = FuncOutput + channelCount };

private:
  std::vector<FuncChannel> channels;
};
} // namespace dhe
