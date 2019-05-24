#pragma once

#include "Module.h"
#include "func/FuncChannel.h"

#include <memory>

namespace dhe {
class Func : public Module {
public:
  Func();

  void process(const ProcessArgs &args) override;

  enum ParameterIds { OperandKnob, OperatorSwitch, AdditionRangeSwitch, MultiplicationRangeSwitch, ParameterCount };

  enum InputIds { FuncInput, InputCount };

  enum OutputIds { FuncOutput, OutputCount };

  std::unique_ptr<FuncChannel> channel;
};
} // namespace dhe
