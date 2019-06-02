#pragma once
#include "envelopes/StageStateMachine.h"
#include "modules/components/Taper.h"
#include "modules/controls/Controls.h"
#include "modules/controls/CurvatureControls.h"
#include "modules/controls/Duration.h"
#include "modules/controls/Inputs.h"
#include "modules/controls/Level.h"
#include "modules/controls/TaperControls.h"

#include <engine/Module.hpp>

namespace dhe {

class BoosterStage : public rack::engine::Module {
public:
  BoosterStage();
  void process(const ProcessArgs &args) override;

  enum ParameterIds {
    ActiveButton,
    CurveKnob,
    DeferButton,
    DurationKnob,
    DurationRangeSwitch,
    EocButton,
    LevelKnob,
    LevelRangeSwitch,
    ShapeSwitch,
    TriggerButton,
    ParameterCount
  };

  enum InputIds { CurveCvInput, DeferInput, DurationCvInput, LevelCvInput, EnvelopeInput, TriggerInput, InputCount };

  enum OutputIds { ActiveOutput, EocOutput, EnvelopeOutput, OutputCount };

private:
  auto envelopeIn() const -> float { return inputVoltage(this, EnvelopeInput); }
  auto activeButton() const -> bool { return buttonIsPressed(this, ActiveButton); }
  auto eocButton() const -> bool { return buttonIsPressed(this, EocButton); }

  auto level() const -> float {
    return scaledRotation<2>(this, LevelKnob, LevelCvInput, LevelRangeSwitch, level::ranges);
  }

  auto taper(float input) const -> float {
    auto const curvature = dhe::curvature(this, CurveKnob, CurveCvInput);
    auto const taper = selectedTaper(this, ShapeSwitch);
    return taper->apply(input, curvature);
  }

  void forward() { sendOut(envelopeIn()); }
  void prepareToGenerate() { startVoltage = envelopeIn(); }
  void generate(float phase) { sendOut(scale(taper(phase), startVoltage, level())); }
  void setActive(bool active) { isActive = active; }
  void setEoc(bool eoc) { isEoc = eoc; }

  void sendActive() {
    auto const voltage = level::unipolarRange.scale(isActive || activeButton());
    outputs[ActiveOutput].setVoltage(voltage);
  }

  void sendEoc() {
    auto const voltage = level::unipolarRange.scale(isEoc || eocButton());
    outputs[EocOutput].setVoltage(voltage);
  }
  void sendOut(float voltage) { outputs[EnvelopeOutput].setVoltage(voltage); }

  StageStateMachine stateMachine{
      inputIsConnectedFunction(this, DeferInput),
      inputIsHighOrButtonIsPressedFunction(this, DeferInput, DeferButton),
      inputIsHighOrButtonIsPressedFunction(this, TriggerInput, TriggerButton),
      duration::withSelectableRange(this, DurationKnob, DurationCvInput, DurationRangeSwitch),
      [this](float /*unused*/) { forward(); },
      [this]() { prepareToGenerate(); },
      [this](float phase) { generate(phase); },
      [this](bool active) { setActive(active); },
      [this](bool eoc) { setEoc(eoc); }};

  bool isActive{false};
  bool isEoc{false};
  float startVoltage{0.F};
};
} // namespace dhe
