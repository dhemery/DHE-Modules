#pragma once

#include "modules/components/FunctionLatch.h"
#include "modules/controls/CommonConfig.h"
#include "modules/controls/CurvatureConfig.h"
#include "modules/controls/DurationConfig.h"
#include "modules/controls/LevelConfig.h"
#include "modules/curve-sequencer/ModuleStep.h"
#include "modules/curve-sequencer/Sequence.h"
#include "modules/curve-sequencer/Step.h"

#include <engine/Module.hpp>
#include <memory>

namespace dhe {

template <int N> class CurveSequencer : public rack::engine::Module {
  using Step = curve_sequencer::Step;
  template <typename T> using PtrT = std::unique_ptr<T>;
  using Sequence = curve_sequencer::Sequence<PtrT>;

  template <typename M> static auto makeSequence(M *module, int stepCount) -> std::unique_ptr<Sequence> {
    auto runLatch = PtrT<Latch>{new FunctionLatch([module]() -> bool { return module->isRunning(); })};
    auto gateLatch = std::unique_ptr<Latch>{new FunctionLatch([module]() -> bool { return module->gate(); })};

    std::vector<std::unique_ptr<Step>> steps;
    for (int i = 0; i < stepCount; i++) {
      steps.emplace_back(new curve_sequencer::ModuleStep<M>(module, i));
    }
    auto sequence = new Sequence{std::move(runLatch), std::move(gateLatch), std::move(steps)};
    runLatch.release();
    gateLatch.release();
    return std::unique_ptr<Sequence>{sequence};
  }

public:
  CurveSequencer();
  ~CurveSequencer() override = default;

  void process(const ProcessArgs &args) override { sequence->process(args.sampleRate); };

  auto isRunning() const -> bool { return inputIsHigh(inputs[RunInput]) || buttonIsPressed(params[RunButton]); }

  auto gate() const -> bool { return inputIsHigh(inputs[GateInput]) || buttonIsPressed(params[GateButton]); }

  void setGenerating(int step, bool state) { lights[GeneratingLights + step].setBrightness(state ? 10.F : 0.F); }

  auto isEnabled(int step) const -> bool {
    return inputIsHigh(inputs[EnabledInputs + step]) || buttonIsPressed(params[EnabledButtons + step]);
  };

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
  std::unique_ptr<Sequence> sequence = makeSequence<CurveSequencer<N>>(this, N);
};

template <int N> CurveSequencer<N>::CurveSequencer() {
  config(ParameterCount, InputCount, OutputCount, LightCount);

  configButton(this, RunButton, "Run", {"RUN input", "Yes"}, 1);
  configButton(this, GateButton, "Gate", {"GATE input", "High"}, 0);
  configButton(this, ResetButton, "Reset", {"RESET input", "High"}, 0);

  configParam(StartKnob, 1.F, N, 1.F, "Start step");
  configParam(StepsKnob, 1.F, N, N, "Steps");

  configLevelRangeSwitch(this, LevelRangeSwitch);
  configDurationRangeSwitch(this, DurationRangeSwitch);

  for (int step = 0; step < N; step++) {
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

    lights[GeneratingLights + step].setBrightness(0.F);
    lights[SustainingLights + step].setBrightness(0.F);
  }
}
} // namespace dhe
