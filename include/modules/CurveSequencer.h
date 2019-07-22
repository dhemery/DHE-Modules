#pragma once

#include <engine/Module.hpp>
#include <modules/controls/CurvatureConfig.h>
#include <modules/controls/DurationConfig.h>
#include <modules/controls/LevelConfig.h>

namespace dhe {

template <int NS> class CurveSequencer : public rack::engine::Module {
public:
  CurveSequencer() {
    config(ParameterCount, InputCount, OutputCount);
    configButton(this, RunButton, "Running", {"When input is high", "Yes"}, 1);
    configButton(this, GateButton, "Gate", {"Tracks input voltage", "High"}, 0);
    configButton(this, ResetButton, "Reset", {"When input rises", "High"}, 0);
    configButton(this, LoopButton, "Looping", {"When input is high", "Yes"}, 0);

    configKnob(this, StartKnob, "Start step", "", Range{1.F, NS}, 0.F);
    configKnob(this, StepsKnob, "Steps", "", Range{1.F, NS}, 1.F);

    configLevelRangeSwitch(this, LevelRangeSwitch);
    configDurationRangeSwitch(this, DurationRangeSwitch);

    for (int step = 0; step < NS; step++) {
      auto const stepDescription = std::string{"Step "} + std::to_string(step + 1);
      configButton(this, EnabledButtons + step, stepDescription + " enabled", {"When input is high", "Yes"}, 1);
      configToggle<6>(this, ModeSwitches + step, stepDescription + " ends when",
                      {"Gate rises", "Gate falls", "Gate changes", "Curve completes", "Gate is low while step holds",
                       "Gate is high while step holds"},
                      3);
      configLevelKnob(this, LevelKnobs + step, LevelRangeSwitch, stepDescription + " level");
      configCurveShapeSwitch(this, ShapeSwitches + step, stepDescription + " shape");
      configCurvatureKnob(this, CurveKnobs + step, stepDescription + " curvature");
      configDurationKnob(this, DurationKnobs + step, DurationRangeSwitch, stepDescription + " duration");
    }
  }

  void process(const ProcessArgs &args) override {}

  enum ParameterIds {
    RunButton,
    GateButton,
    ResetButton,
    LoopButton,
    StartKnob,
    StepsKnob,
    LevelRangeSwitch,
    DurationRangeSwitch,
    ENUMS(EnabledButtons, NS),
    ENUMS(ShapeSwitches, NS),
    ENUMS(LevelKnobs, NS),
    ENUMS(CurveKnobs, NS),
    ENUMS(DurationKnobs, NS),
    ENUMS(ModeSwitches, NS),
    ParameterCount
  };

  enum InputIds {
    RunInput,
    LoopInput,
    ResetInput,
    GateInput,
    StartCVInput,
    StepsCVInput,
    ENUMS(EnabledInputs, NS),
    InputCount
  };

  enum OutputIds { EOCOutput, OutOutput, ENUMS(EosOutputs, NS), OutputCount };
};
} // namespace dhe
