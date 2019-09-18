#include "modules/CurveSequencerModule.h"

#include "modules/controls/CurvatureConfig.h"
#include "modules/controls/DurationConfig.h"
#include "modules/controls/LevelConfig.h"

namespace dhe {

template <int NS> bool CurveSequencerModule<NS>::isRunning() { return false; }

template <int NS> void CurveSequencerModule<NS>::process(const rack::engine::Module::ProcessArgs &args) {}

template <int NS> void CurveSequencerModule<NS>::send(float voltage) {}

template <int NS> CurveSequencerModule<NS>::CurveSequencerModule() {
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
                    {"End when gate rises", "End when gate falls", "End when gate changes", "Skip/end if gate is high",
                     "Skip/end if gate is low", "Skip"},
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

template class CurveSequencerModule<8>;
template class CurveSequencerModule<16>;
} // namespace dhe