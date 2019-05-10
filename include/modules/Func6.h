#pragma once

#include "Module.h"

#include "func/FuncChannel.h"

namespace DHE {
class Func6 : public Module {
  static constexpr auto channel_count = 6;

public:
  Func6();

  void process(const ProcessArgs &args) override;

  enum ParameterIds {
    KNOB,
    OPERATOR_SWITCH = KNOB + channel_count,
    ADDITION_RANGE_SWITCH = OPERATOR_SWITCH + channel_count,
    MULTIPLICATION_RANGE_SWITCH = ADDITION_RANGE_SWITCH + channel_count,
    PARAMETER_COUNT = MULTIPLICATION_RANGE_SWITCH + channel_count
  };

  enum InputIds { IN, INPUT_COUNT = IN + channel_count };

  enum OutputIds { OUT, OUTPUT_COUNT = OUT + channel_count };

  std::vector<FuncChannel> channels{};
};
} // namespace DHE
