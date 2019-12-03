#pragma once

#include <common.hpp>

namespace dhe {
namespace ranger {
  class RangerControls {
  public:
    enum ParameterIds {
      LevelKnob,
      LevelAvKnob,
      CcwLimitKnob,
      CcwLimitAvKnob,
      CcwLimitRangeSwitch,
      CwLimitKnob,
      CwLimitAvKnob,
      CwLimitRangeSwitch,
      ParameterCount
    };
    enum InputIds { LevelCvInput, CcwLimitCvInput, CwLimitCvInput, InputCount };
    enum OutputIds { RangerOutput, OutputCount };
  };

} // namespace ranger
} // namespace dhe