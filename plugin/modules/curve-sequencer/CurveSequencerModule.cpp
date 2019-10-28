#include "CurveSequencerModule.h"

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
    config(ParameterCount, InputCount, OutputCount, LightCount);

    configButton(this, RunButton, "Run", {"RUN input", "Yes"}, 1);
    configButton(this, GateButton, "Gate", {"GATE input", "High"}, 0);
    configButton(this, ResetButton, "Reset", {"RESET input", "High"}, 0);

    configParam(StartKnob, 1.F, N, 1.F, "Start step");
    configParam(StepsKnob, 1.F, N, N, "Sequence length", " steps");

    configLevelRangeSwitch(this, LevelRangeSwitch);
    configDurationRangeSwitch(this, DurationRangeSwitch);

    for (int stepIndex = 0; stepIndex < N; stepIndex++) {
      configToggle<generateModeCount>(this, GenerateModeSwitches + stepIndex, "Generate mode", generateModeDescriptions,
                                      generateDefaultMode);
      configToggle<sustainModeCount>(this, SustainModeSwitches + stepIndex, "Sustain mode", sustainModeDescriptions,
                                     sustainDefaultMode);
      configLevelKnob(this, LevelKnobs + stepIndex, LevelRangeSwitch, "Level");
      configCurveShapeSwitch(this, ShapeSwitches + stepIndex, "Shape");
      configCurvatureKnob(this, CurveKnobs + stepIndex, "Curvature");
      configDurationKnob(this, DurationKnobs + stepIndex, DurationRangeSwitch, "Duration");
      configButton(this, EnabledButtons + stepIndex, "Enabled", {"ENABLED input", "Yes"}, 1);

      lights[GeneratingLights + stepIndex].setBrightness(0.F);
      lights[SustainingLights + stepIndex].setBrightness(0.F);
    }

    sequencer.reset(new CurveSequencer<CurveSequencerModule<N>>(*this, N));
  }

  template <int N> auto CurveSequencerModule<N>::curvature(int stepIndex) const -> float {
    return dhe::curvature(params[CurveKnobs + stepIndex]);
  }

  template <int N> auto CurveSequencerModule<N>::duration(int stepIndex) const -> float {
    return selectableDuration(params[DurationKnobs + stepIndex], params[DurationRangeSwitch]);
  }

  template <int N> auto CurveSequencerModule<N>::gate() const -> bool {
    return isHigh(inputs[GateInput]) || isPressed(params[GateButton]);
  }

  template <int N> auto CurveSequencerModule<N>::generateMode(int stepIndex) const -> int {
    return positionOf(params[GenerateModeSwitches + stepIndex]);
  }

  template <int N> auto CurveSequencerModule<N>::isEnabled(int stepIndex) const -> bool {
    return isHigh(inputs[EnabledInputs + stepIndex]) || isPressed(params[EnabledButtons + stepIndex]);
  }

  template <int N> auto CurveSequencerModule<N>::isRunning() const -> bool {
    return isHigh(inputs[RunInput]) || isPressed(params[RunButton]);
  }

  template <int N> auto CurveSequencerModule<N>::level(int stepIndex) const -> float {
    return selectableLevel(params[LevelKnobs + stepIndex], params[LevelRangeSwitch]);
  }

  template <int N> auto CurveSequencerModule<N>::output() const -> float {
    return const_cast<rack::engine::Output &>(outputs[OutOutput]).getVoltage();
  }

  template <int N> void CurveSequencerModule<N>::process(const ProcessArgs &args) {
    sequencer->execute(args.sampleTime); }

  template <int N> auto CurveSequencerModule<N>::selectionLength() const -> int { return valueOf(params[StepsKnob]); }

  template <int N> auto CurveSequencerModule<N>::selectionStart() const -> int {
    return valueOf(params[StartKnob]) - 1;
  }

  template <int N> void CurveSequencerModule<N>::setGenerating(int stepIndex, bool state) {
    lights[GeneratingLights + stepIndex].setBrightness(state ? 10.F : 0.F);
  }

  template <int N> void CurveSequencerModule<N>::setOutput(float voltage) { outputs[OutOutput].setVoltage(voltage); }

  template <int N> void CurveSequencerModule<N>::setSustaining(int stepIndex, bool state) {
    lights[SustainingLights + stepIndex].setBrightness(state ? 10.F : 0.F);
  }

  template <int N> auto CurveSequencerModule<N>::sustainMode(int stepIndex) const -> int {
    return positionOf(params[SustainModeSwitches + stepIndex]);
  }

  template <int N> auto CurveSequencerModule<N>::taperSelection(int stepIndex) const -> int {
    return positionOf(params[ShapeSwitches + stepIndex]);
  }

  template class CurveSequencerModule<4>;
  template class CurveSequencerModule<8>;
  template class CurveSequencerModule<16>;
} // namespace curve_sequencer
} // namespace dhe
