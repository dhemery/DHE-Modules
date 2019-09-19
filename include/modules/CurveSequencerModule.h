#pragma once

#include "modules/CurveSequencer.h"

#include <engine/Module.hpp>

namespace dhe {

template <int NS> class CurveSequencerModule : public rack::engine::Module, public CurveSequencer {
public:
  CurveSequencerModule();

  auto gate() -> int override;
  auto isRunning() -> bool override;
  void process(const ProcessArgs &args) override;
  void send(float voltage) override;
  auto startStep() -> int override;

  enum ParameterIds {
    DurationRangeSwitch,
    GateButton,
    LevelRangeSwitch,
    ResetButton,
    RunButton,
    StartKnob,
    StepsKnob,
    ENUMS(CurveKnobs, NS),
    ENUMS(DurationKnobs, NS),
    ENUMS(EnabledButtons, NS),
    ENUMS(LevelKnobs, NS),
    ENUMS(GenerateModeSwitches, NS),
    ENUMS(SustainModeSwitches, NS),
    ENUMS(ShapeSwitches, NS),
    ParameterCount
  };

  enum InputIds { GateInput, ResetInput, RunInput, StartCVInput, StepsCVInput, ENUMS(EnabledInputs, NS), InputCount };

  enum OutputIds { OutOutput, OutputCount };

  enum LightIds { ENUMS(GeneratingLights, NS), ENUMS(SustainingLights, NS), LightCount };
};
} // namespace dhe
