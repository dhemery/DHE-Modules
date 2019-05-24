#pragma once

#include "Module.h"
#include "func/FuncChannel.h"

namespace dhe {
class Func : public Module {
public:
  Func();

  void process(const ProcessArgs &args) override;

  enum ParameterIds { OperandKnob, OperatorSwitch, AdditionRangeSwitch, MultiplicationRangeSwitch, ParameterCount };

  enum InputIds { FuncInput, InputCount };

  enum OutputIds { FuncOutput, OutputCount };

  FuncChannel channel{};
};
} // namespace dhe
