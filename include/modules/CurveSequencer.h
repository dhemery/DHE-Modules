#pragma once

#include "modules/components/FunctionLatch.h"
#include "modules/controls/CommonConfig.h"
#include "modules/controls/CurvatureConfig.h"
#include "modules/controls/DurationConfig.h"
#include "modules/controls/LevelConfig.h"
#include "modules/curve-sequencer/IndexedStep.h"
#include "modules/curve-sequencer/Sequence.h"
#include "modules/curve-sequencer/SequenceControls.h"
#include "modules/curve-sequencer/Step.h"
#include "modules/curve-sequencer/StepControls.h"

#include <engine/Module.hpp>
#include <memory>

namespace dhe {

template <int N>
class CurveSequencer : public rack::engine::Module,
                       public curve_sequencer::SequenceControls,
                       public curve_sequencer::StepControls {
  using Step = curve_sequencer::Step;
  using Sequence = curve_sequencer::Sequence;
  using IndexedStep = curve_sequencer::IndexedStep;

public:
  CurveSequencer();
  ~CurveSequencer() override = default;

  void process(const ProcessArgs &args) override { sequence.process(args.sampleRate); };

  auto isRunning() const -> bool override {
    return inputIsHigh(rack::engine::Module::inputs[RunInput])
           || buttonIsPressed(rack::engine::Module::params[RunButton]);
  }

  auto gate() const -> bool override {
    return inputIsHigh(rack::engine::Module::inputs[GateInput])
           || buttonIsPressed(rack::engine::Module::params[GateButton]);
  }

  auto selectionLength() const -> int override { return paramValue(rack::engine::Module::params[StepsKnob]); }

  auto selectionStart() const -> int override { return paramValue(rack::engine::Module::params[StartKnob]) - 1; }

  void setGenerating(int step, bool state) override {
    rack::engine::Module::lights[GeneratingLights + step].setBrightness(state ? 10.F : 0.F);
  }
  void setSustaining(int step, bool state) override {
    rack::engine::Module::lights[SustainingLights + step].setBrightness(state ? 10.F : 0.F);
  }
  auto generateMode(int step) const -> int override {
    return static_cast<int>(paramValue(rack::engine::Module::params[GenerateModeSwitches + step]));
  }
  auto sustainMode(int step) const -> int override {
    return static_cast<int>(paramValue(rack::engine::Module::params[SustainModeSwitches + step]));
  }
  auto isEnabled(int step) const -> bool override {
    return inputIsHigh(rack::engine::Module::inputs[EnabledInputs + step])
           || buttonIsPressed(rack::engine::Module::params[EnabledButtons + step]);
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
    for (int i = 0; i < N; i++) {
      steps.emplace_back(new IndexedStep(stepControls, i));
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
  configParam(StepsKnob, 1.F, N, N, "Steps");

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

  for (int step = 0; step < N; step++) {
    configToggle<7>(this, GenerateModeSwitches + step, "Generate mode", generateModeNames, 6);
    configToggle<6>(this, SustainModeSwitches + step, "Sustain mode", sustainModeNames, 5);
    configLevelKnob(this, LevelKnobs + step, LevelRangeSwitch, "Level");
    configCurveShapeSwitch(this, ShapeSwitches + step, "Shape");
    configCurvatureKnob(this, CurveKnobs + step, "Curvature");
    configDurationKnob(this, DurationKnobs + step, DurationRangeSwitch, "Duration");
    configButton(this, EnabledButtons + step, "Enabled", {"ENABLED input", "Yes"}, 1);

    lights[GeneratingLights + step].setBrightness(0.F);
    lights[SustainingLights + step].setBrightness(0.F);
  }
}
} // namespace dhe
