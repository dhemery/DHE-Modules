#pragma once

#include "modules/curve-sequencer/Sequence.h"

#include <engine/Module.hpp>
#include <memory>

namespace dhe {

template <int N> class CurveSequencer : public rack::engine::Module {
public:
  CurveSequencer();
  ~CurveSequencer() override = default;

  void process(const ProcessArgs &args) override;

  auto gate() const -> bool;
  auto isRunning() const -> bool;
  auto output() const -> float;
  void setOutput(float voltage);

  auto curvature(int stepIndex) const -> float;
  auto duration(int stepIndex) const -> float;
  auto generateMode(int stepIndex) const -> int;
  auto isEnabled(int stepIndex) const -> bool;
  auto level(int stepIndex) const -> float;
  auto selectionLength() const -> int;
  auto selectionStart() const -> int;
  void setGenerating(int stepIndex, bool state);
  void setSustaining(int stepIndex, bool state);
  auto sustainMode(int stepIndex) const -> int;
  auto taperSelection(int stepIndex) const -> int;

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
  curve_sequencer::Sequence<CurveSequencer<N>> sequence{*this, N};
};

} // namespace dhe
