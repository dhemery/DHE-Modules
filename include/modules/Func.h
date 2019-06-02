#pragma once

#include "func/FuncChannel.h"

#include <engine/Module.hpp>
#include <memory>

namespace dhe {
class Func : public rack::engine::Module {
public:
  Func();

  void process(const ProcessArgs &args) override;

  enum ParameterIds { AmountKnob, OperationSwitch, OffsetRangeSwitch, MultiplierRangeSwitch, ParameterCount };

  enum InputIds { FuncInput, InputCount };

  enum OutputIds { FuncOutput, OutputCount };

private:
  std::unique_ptr<FuncChannel> channel;
};
} // namespace dhe
