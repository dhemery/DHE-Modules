#pragma once

#include <functional>

#include "Module.h"

namespace dhe {

class Ranger : public Module {
public:
  Ranger();

  void process(const ProcessArgs &args) override;

  enum ParameterIds {
    LevelKnob,
    LevelAv,
    CcwLimitKnob,
    CcwLimitAv,
    CcwLimitRangeSwitch,
    CwLimitKnob,
    CwLimitAv,
    CwLimitRangeSwitch,
    ParameterCount
  };
  enum InputIds { LevelCv, CcwLimitCv, CwLimitCv, InputCount };
  enum OutputIds { MainOut, OutputCount };

private:
  std::function<float()> ccwLimit;
  std::function<float()> cwLimit;
  std::function<float()> level;
};

} // namespace dhe
