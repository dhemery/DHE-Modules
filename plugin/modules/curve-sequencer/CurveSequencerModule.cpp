#include "CurveSequencerModule.h"

#include "CurveSequencerControls.h"
#include "config/CommonConfig.h"
#include "config/CurvatureConfig.h"
#include "config/DurationConfig.h"
#include "config/LevelConfig.h"
#include "controls/CurvatureInputs.h"
#include "controls/DurationInputs.h"
#include "controls/LevelInputs.h"

namespace dhe {

namespace curve_sequencer {

  using rack::engine::Module;

  template <int N> CurveSequencerModule<N>::CurveSequencerModule() {
    config(CurveSequencerControls<N>::ParameterCount, CurveSequencerControls<N>::InputCount,
           CurveSequencerControls<N>::OutputCount, CurveSequencerControls<N>::LightCount);

    configButton(this, CurveSequencerControls<N>::RunButton, "Run", {"RUN input", "Yes"}, 1);
    configButton(this, CurveSequencerControls<N>::GateButton, "Gate", {"GATE input", "High"}, 0);
    configButton(this, CurveSequencerControls<N>::ResetButton, "Reset", {"RESET input", "High"}, 0);

    configParam(CurveSequencerControls<N>::StartKnob, 1.F, N, 1.F, "Start step");
    configParam(CurveSequencerControls<N>::StepsKnob, 1.F, N, N, "Sequence length", " steps");

    configLevelRangeSwitch(this, CurveSequencerControls<N>::LevelRangeSwitch);
    configDurationRangeSwitch(this, CurveSequencerControls<N>::DurationRangeSwitch);

    for (int stepIndex = 0; stepIndex < N; stepIndex++) {
      configToggle<generateModeCount>(this, CurveSequencerControls<N>::GenerateModeSwitches + stepIndex,
                                      "Generate mode", generateModeDescriptions, generateDefaultMode);
      configToggle<sustainModeCount>(this, CurveSequencerControls<N>::SustainModeSwitches + stepIndex, "Sustain mode",
                                     sustainModeDescriptions, sustainDefaultMode);
      configLevelKnob(this, CurveSequencerControls<N>::LevelKnobs + stepIndex,
                      CurveSequencerControls<N>::LevelRangeSwitch, "Level");
      configCurveShapeSwitch(this, CurveSequencerControls<N>::ShapeSwitches + stepIndex, "Shape");
      configCurvatureKnob(this, CurveSequencerControls<N>::CurveKnobs + stepIndex, "Curvature");
      configDurationKnob(this, CurveSequencerControls<N>::DurationKnobs + stepIndex,
                         CurveSequencerControls<N>::DurationRangeSwitch, "Duration");
      configButton(this, CurveSequencerControls<N>::EnabledButtons + stepIndex, "Enabled", {"ENABLED input", "Yes"}, 1);

      lights[CurveSequencerControls<N>::GeneratingLights + stepIndex].setBrightness(0.F);
      lights[CurveSequencerControls<N>::SustainingLights + stepIndex].setBrightness(0.F);
    }
  }

  template <int N> auto CurveSequencerModule<N>::curvature(int stepIndex) const -> float {
    return dhe::curvature(params[CurveSequencerControls<N>::CurveKnobs + stepIndex]);
  }

  template <int N> auto CurveSequencerModule<N>::duration(int stepIndex) const -> float {
    return selectableDuration(params[CurveSequencerControls<N>::DurationKnobs + stepIndex],
                              params[CurveSequencerControls<N>::DurationRangeSwitch]);
  }

  template <int N> auto CurveSequencerModule<N>::level(int stepIndex) const -> float {
    return selectableLevel(params[CurveSequencerControls<N>::LevelKnobs + stepIndex],
                           params[CurveSequencerControls<N>::LevelRangeSwitch]);
  }

  template <int N> auto CurveSequencerModule<N>::output() const -> float {
    return const_cast<rack::engine::Output &>(outputs[CurveSequencerControls<N>::OutOutput]).getVoltage();
  }

  template <int N> void CurveSequencerModule<N>::setOutput(float voltage) {
    outputs[CurveSequencerControls<N>::OutOutput].setVoltage(voltage);
  }

  template <int N> auto CurveSequencerModule<N>::taperSelection(int stepIndex) const -> int {
    return positionOf(params[CurveSequencerControls<N>::ShapeSwitches + stepIndex]);
  }

  template class CurveSequencerModule<4>;
  template class CurveSequencerModule<8>;
  template class CurveSequencerModule<16>;
} // namespace curve_sequencer
} // namespace dhe
