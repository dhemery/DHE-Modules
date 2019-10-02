#pragma once

#include "modules/components/FunctionLatch.h"
#include "modules/controls/CommonConfig.h"
#include "modules/controls/CurvatureConfig.h"
#include "modules/controls/DurationConfig.h"
#include "modules/controls/LevelConfig.h"
#include "modules/curve-sequencer/ComboStep.h"
#include "modules/curve-sequencer/GenerateStep.h"
#include "modules/curve-sequencer/Sequence.h"
#include "modules/curve-sequencer/SequenceControls.h"
#include "modules/curve-sequencer/Step.h"
#include "modules/curve-sequencer/StepControls.h"
#include "modules/curve-sequencer/SustainStep.h"

#include <engine/Module.hpp>
#include <memory>

namespace dhe {

template <int N>
class CurveSequencer : public rack::engine::Module,
                       public curve_sequencer::SequenceControls,
                       public curve_sequencer::StepControls {
  using Step = curve_sequencer::Step;
  using Sequence = curve_sequencer::Sequence;
  using ComboStep = curve_sequencer::ComboStep;

public:
  CurveSequencer();
  ~CurveSequencer() override = default;

  auto gate() const -> bool override {
    return inputIsHigh(rack::engine::Module::inputs[GateInput])
           || buttonIsPressed(rack::engine::Module::params[GateButton]);
  }

  auto generateMode(int stepIndex) const -> int override {
    return static_cast<int>(paramValue(rack::engine::Module::params[GenerateModeSwitches + stepIndex]));
  }

  auto isEnabled(int stepIndex) const -> bool override {
    return inputIsHigh(rack::engine::Module::inputs[EnabledInputs + stepIndex])
           || buttonIsPressed(rack::engine::Module::params[EnabledButtons + stepIndex]);
  }

  auto isRunning() const -> bool override {
    return inputIsHigh(rack::engine::Module::inputs[RunInput])
           || buttonIsPressed(rack::engine::Module::params[RunButton]);
  }

  void process(const ProcessArgs &args) override { sequence.process(args.sampleRate); };

  auto selectionLength() const -> int override { return paramValue(rack::engine::Module::params[StepsKnob]); }

  auto selectionStart() const -> int override { return paramValue(rack::engine::Module::params[StartKnob]) - 1; }

  void setGenerating(int stepIndex, bool state) override {
    rack::engine::Module::lights[GeneratingLights + stepIndex].setBrightness(state ? 10.F : 0.F);
  }

  void setSustaining(int stepIndex, bool state) override {
    rack::engine::Module::lights[SustainingLights + stepIndex].setBrightness(state ? 10.F : 0.F);
  }
  auto sustainMode(int stepIndex) const -> int override {
    return static_cast<int>(paramValue(rack::engine::Module::params[SustainModeSwitches + stepIndex]));
  }

  enum ParameterIds {
    DurationRangeSwitch,
    GateButton,
    LevelRangeSwitch,
    ResetButton,
    RunButton,
    StartKnob,
    StepsKnob,
    ENUMS(CurveKnobs, N),
    ENUMS(DurationKnobs, N),
    ENUMS(EnabledButtons, N),
    ENUMS(LevelKnobs, N),
    ENUMS(GenerateModeSwitches, N),
    ENUMS(SustainModeSwitches, N),
    ENUMS(ShapeSwitches, N),
    ParameterCount
  };

  enum InputIds { GateInput, ResetInput, RunInput, StartCVInput, StepsCVInput, ENUMS(EnabledInputs, N), InputCount };

  enum OutputIds { OutOutput, OutputCount };

  enum LightIds { ENUMS(GeneratingLights, N), ENUMS(SustainingLights, N), LightCount };

private:
  FunctionLatch gateLatch{[this]() -> bool { return gate(); }};
  FunctionLatch runLatch{[this]() -> bool { return isRunning(); }};
  std::vector<std::unique_ptr<Step>> steps = makeSteps(*this);
  Sequence sequence{*this, runLatch, gateLatch, steps};

  static auto makeSteps(StepControls &stepControls) -> std::vector<std::unique_ptr<Step>> {
    std::vector<std::unique_ptr<Step>> steps{};
    steps.reserve(N);
    for (int stepIndex = 0; stepIndex < N; stepIndex++) {
      steps.emplace_back(new ComboStep(stepControls, stepIndex));
    }
    return steps;
  }
};

enum Modes { Rise, Fall, Edge, High, Low, Skip, FullDuration, ModeCount };

template <int N> CurveSequencer<N>::CurveSequencer() {
  config(ParameterCount, InputCount, OutputCount, LightCount);

  configButton(this, RunButton, "Run", {"RUN input", "Yes"}, 1);
  configButton(this, GateButton, "Gate", {"GATE input", "High"}, 0);
  configButton(this, ResetButton, "Reset", {"RESET input", "High"}, 0);

  configParam(StartKnob, 1.F, N, 1.F, "Start step");
  configParam(StepsKnob, 1.F, N, N, "Sequence length", " steps");

  configLevelRangeSwitch(this, LevelRangeSwitch);
  configDurationRangeSwitch(this, DurationRangeSwitch);

  static std::array<std::string, ModeCount> generateModeNames{"Interrupt if gate rises",
                                                              "Interrupt if gate falls",
                                                              "Interrupt if gate changes",
                                                              "Skip/interrupt if gate is high",
                                                              "Skip/interrupt if gate is low",
                                                              "Skip",
                                                              "Run full duration"};

  static std::array<std::string, ModeCount - 1> sustainModeNames{};
  std::copy_n(generateModeNames.begin(), sustainModeNames.size(), sustainModeNames.begin());

  for (int stepIndex = 0; stepIndex < N; stepIndex++) {
    configToggle<7>(this, GenerateModeSwitches + stepIndex, "Generate mode", generateModeNames, 6);
    configToggle<6>(this, SustainModeSwitches + stepIndex, "Sustain mode", sustainModeNames, 5);
    configLevelKnob(this, LevelKnobs + stepIndex, LevelRangeSwitch, "Level");
    configCurveShapeSwitch(this, ShapeSwitches + stepIndex, "Shape");
    configCurvatureKnob(this, CurveKnobs + stepIndex, "Curvature");
    configDurationKnob(this, DurationKnobs + stepIndex, DurationRangeSwitch, "Duration");
    configButton(this, EnabledButtons + stepIndex, "Enabled", {"ENABLED input", "Yes"}, 1);

    lights[GeneratingLights + stepIndex].setBrightness(0.F);
    lights[SustainingLights + stepIndex].setBrightness(0.F);
  }
}
} // namespace dhe
