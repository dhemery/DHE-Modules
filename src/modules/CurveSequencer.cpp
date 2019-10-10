#include "modules/CurveSequencer.h"

#include "modules/controls/CommonConfig.h"
#include "modules/controls/CurvatureConfig.h"
#include "modules/controls/DurationConfig.h"
#include "modules/controls/LevelConfig.h"
#include "modules/curve-sequencer/ComboStep.h"

namespace dhe {

using rack::engine::Module;

template <int N> CurveSequencer<N>::CurveSequencer() : sequence{*this, steps, N} {
  config(ParameterCount, InputCount, OutputCount, LightCount);

  configButton(this, RunButton, "Run", {"RUN input", "Yes"}, 1);
  configButton(this, GateButton, "Gate", {"GATE input", "High"}, 0);
  configButton(this, ResetButton, "Reset", {"RESET input", "High"}, 0);

  configParam(StartKnob, 1.F, N, 1.F, "Start step");
  configParam(StepsKnob, 1.F, N, N, "Sequence length", " steps");

  configLevelRangeSwitch(this, LevelRangeSwitch);
  configDurationRangeSwitch(this, DurationRangeSwitch);

  constexpr auto generateModeCount = static_cast<size_t>(curve_sequencer::Step::Mode::Count);
  constexpr auto sustainModeCount = generateModeCount - 1;
  constexpr auto durationMode = static_cast<int>(curve_sequencer::Step::Mode::Duration);
  constexpr auto skipMode = static_cast<int>(curve_sequencer::Step::Mode::Skip);

  auto generateModeNames = std::array<std::string, generateModeCount>{"Interrupt if gate rises",
                                                                      "Interrupt if gate falls",
                                                                      "Interrupt if gate changes",
                                                                      "Skip/interrupt if gate is high",
                                                                      "Skip/interrupt if gate is low",
                                                                      "Skip",
                                                                      "Run full duration"};

  auto sustainModeNames = std::array<std::string, sustainModeCount>{};
  std::copy_n(generateModeNames.begin(), sustainModeCount, sustainModeNames.begin());

  steps.reserve(N);

  for (int stepIndex = 0; stepIndex < N; stepIndex++) {
    configToggle<generateModeCount>(this, GenerateModeSwitches + stepIndex, "Generate mode", generateModeNames,
                                    durationMode);
    configToggle<sustainModeCount>(this, SustainModeSwitches + stepIndex, "Sustain mode", sustainModeNames, skipMode);
    configLevelKnob(this, LevelKnobs + stepIndex, LevelRangeSwitch, "Level");
    configCurveShapeSwitch(this, ShapeSwitches + stepIndex, "Shape");
    configCurvatureKnob(this, CurveKnobs + stepIndex, "Curvature");
    configDurationKnob(this, DurationKnobs + stepIndex, DurationRangeSwitch, "Duration");
    configButton(this, EnabledButtons + stepIndex, "Enabled", {"ENABLED input", "Yes"}, 1);

    lights[GeneratingLights + stepIndex].setBrightness(0.F);
    lights[SustainingLights + stepIndex].setBrightness(0.F);

    steps.emplace_back(new curve_sequencer::ComboStep(*this, stepIndex));
  }
}

template <int N> auto CurveSequencer<N>::gate() const -> bool {
  return inputIsHigh(inputs[GateInput]) || buttonIsPressed(params[GateButton]);
}

template <int N> auto CurveSequencer<N>::generateMode(int stepIndex) const -> int {
  return static_cast<int>(paramValue(params[GenerateModeSwitches + stepIndex]));
}

template <int N> auto CurveSequencer<N>::isEnabled(int stepIndex) const -> bool {
  return inputIsHigh(inputs[EnabledInputs + stepIndex]) || buttonIsPressed(params[EnabledButtons + stepIndex]);
}

template <int N> auto CurveSequencer<N>::isRunning() const -> bool {
  return inputIsHigh(inputs[RunInput]) || buttonIsPressed(params[RunButton]);
}

template <int N> void CurveSequencer<N>::process(const ProcessArgs &args) { sequence.process(args.sampleRate); }

template <int N> auto CurveSequencer<N>::selectionLength() const -> int { return paramValue(params[StepsKnob]); }

template <int N> auto CurveSequencer<N>::selectionStart() const -> int { return paramValue(params[StartKnob]) - 1; }

template <int N> void CurveSequencer<N>::setGenerating(int stepIndex, bool state) {
  lights[GeneratingLights + stepIndex].setBrightness(state ? 10.F : 0.F);
}

template <int N> void CurveSequencer<N>::setSustaining(int stepIndex, bool state) {
  lights[SustainingLights + stepIndex].setBrightness(state ? 10.F : 0.F);
}

template <int N> auto CurveSequencer<N>::sustainMode(int stepIndex) const -> int {
  return static_cast<int>(paramValue(params[SustainModeSwitches + stepIndex]));
}

template class CurveSequencer<curve_sequencer::eightSteps>;
template class CurveSequencer<curve_sequencer::sixteenSteps>;
} // namespace dhe
