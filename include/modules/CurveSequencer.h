#pragma once

#include "modules/components/FunctionLatch.h"
#include "modules/curve-sequencer/Sequence.h"
#include "modules/curve-sequencer/SequenceControls.h"
#include "modules/curve-sequencer/Step.h"
#include "modules/curve-sequencer/StepControls.h"
#include "modules/curve-sequencer/StepMode.h"

#include <engine/Module.hpp>
#include <memory>

namespace dhe {

template <int N>
class CurveSequencer : public rack::engine::Module,
                       public curve_sequencer::SequenceControls,
                       public curve_sequencer::StepControls {
  using StepMode = curve_sequencer::StepMode;

public:
  CurveSequencer();
  ~CurveSequencer() override = default;

  auto gate() const -> bool override;
  auto generateMode(int stepIndex) const -> StepMode override;
  auto isEnabled(int stepIndex) const -> bool override;
  auto isRunning() const -> bool override;
  void process(const ProcessArgs &args) override;
  auto selectionLength() const -> int override;
  auto selectionStart() const -> int override;
  void setGenerating(int stepIndex, bool state) override;
  void setSustaining(int stepIndex, bool state) override;
  auto sustainMode(int stepIndex) const -> StepMode override;

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
  std::vector<std::unique_ptr<curve_sequencer::Step>> steps{};
  FunctionLatch runLatch;
  FunctionLatch gateLatch;
  curve_sequencer::Sequence sequence;
};

} // namespace dhe
