#pragma once

#include <engine/Module.hpp>

namespace dhe {

class CurveSequencer8 : public rack::engine::Module {
public:
  auto static constexpr numberOfSteps = 8;

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
    ENUMS(EnabledButtons, numberOfSteps),
    ENUMS(ShapeSwitches, numberOfSteps),
    ENUMS(LevelKnobs, numberOfSteps),
    ENUMS(CurveKnobs, numberOfSteps),
    ENUMS(DurationKnobs, numberOfSteps),
    ENUMS(ModeSwitches, numberOfSteps),
    ParameterCount
  };

  enum InputIds {
    RunInput,
    LoopInput,
    ResetInput,
    GateInput,
    StartCVInput,
    StepsCVInput,
    ENUMS(EnabledInputs, numberOfSteps),
    InputCount
  };

  enum OutputIds {
    EOCOutput,
    OutOutput,
    ENUMS(EosOutputs, numberOfSteps),
    OutputCount
  };
};
} // namespace dhe
