#pragma once

#include <engine/Module.hpp>

namespace dhe {

class CurveSequencer8 : public rack::engine::Module {
public:
  CurveSequencer8();

  void process(const ProcessArgs &args) override;

  enum ParameterIds {
    RunButton,
    GateButton,
    ResetButton,
    LoopButton,
    StartKnob,
    StepsKnob,
    LevelRangeSwitch,
    DurationRangeSwitch,
    ParameterCount
  };

  enum InputIds {
    RunInput,
    LoopInput,
    ResetInput,
    GateInput,
    StartCVInput,
    StepsCVInput,
    InputCount
  };

  enum OutputIds {
    EOCOutput,
    OutOutput,
    OutputCount
  };
};
} // namespace dhe
