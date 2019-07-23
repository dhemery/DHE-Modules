#pragma once

#include <engine/Module.hpp>
#include <modules/controls/CurvatureConfig.h>
#include <modules/controls/DurationConfig.h>
#include <modules/controls/LevelConfig.h>

namespace dhe {

template <int NS> class CurveSequencer : public rack::engine::Module {
public:
  CurveSequencer() {
    config(ParameterCount, InputCount, OutputCount, LightCount);

    configButton(this, RunButton, "Run", {"RUN input", "Yes"}, 1);
    configButton(this, GateButton, "Gate", {"GATE input", "High"}, 0);
    configButton(this, ResetButton, "Reset", {"RESET input", "High"}, 0);

    configParam(StartKnob, 1.F, NS, 1.F, "Start step");
    configParam(StepsKnob, 1.F, NS, NS, "Steps");

    configLevelRangeSwitch(this, LevelRangeSwitch);
    configDurationRangeSwitch(this, DurationRangeSwitch);

    for (int step = 0; step < NS; step++) {
      configToggle<7>(this, GenerateModeSwitches + step, "Generate mode",
                      {"Interrupt if gate rises", "Interrupt if gate falls", "Interrupt if gate changes",
                       "Skip/interrupt if gate is high", "Skip/interrupt if gate is low", "Skip", "Run to completion"},
                      6);
      configToggle<6>(this, SustainModeSwitches + step, "Sustain mode",
                      {"End when gate rises", "End when gate falls", "End when gate changes",
                       "Skip/end if gate is high", "Skip/end if gate is low", "Skip"},
                      5);
      configLevelKnob(this, LevelKnobs + step, LevelRangeSwitch, "Level");
      configCurveShapeSwitch(this, ShapeSwitches + step, "Shape");
      configCurvatureKnob(this, CurveKnobs + step, "Curvature");
      configDurationKnob(this, DurationKnobs + step, DurationRangeSwitch, "Duration");
      configButton(this, EnabledButtons + step, "Enabled", {"ENABLED input", "Yes"}, 1);
      auto const intensity = ((float) step) / ((float) NS);
      lights[GeneratingLights + step].setBrightness(intensity);
      lights[SustainingLights + step].setBrightness(1.F - intensity);
    }
  }

  void process(const ProcessArgs &args) override {}

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

  enum InputIds {
    GateInput,
    ResetInput,
    RunInput,
    StartCVInput,
    StepsCVInput,
    ENUMS(EnabledInputs, NS),
    InputCount
  };

  enum OutputIds { OutOutput, OutputCount };

  enum LightIds { ENUMS(GeneratingLights, NS), ENUMS(SustainingLights, NS), LightCount };
};
} // namespace dhe
