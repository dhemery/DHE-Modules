#pragma once
#include "envelopes/StageStateMachine.h"
#include "modules/components/Taper.h"
#include "modules/controls/CommonInputs.h"
#include "modules/controls/CurvatureInputs.h"
#include "modules/controls/DurationInputs.h"
#include "modules/controls/LevelInputs.h"

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
  auto activeButton() const -> bool { return buttonIsPressed(this, ActiveButton); }

  auto deferIsConnected() const -> bool { return inputIsConnected(this, DeferInput); }

  auto duration() const -> float {
    return selectableDuration(this, DurationKnob, DurationCvInput, DurationRangeSwitch);
  }

  auto envelopeIn() const -> float { return inputVoltage(this, EnvelopeInput); }

  auto eocButton() const -> bool { return buttonIsPressed(this, EocButton); }

  void forward() { sendOut(envelopeIn()); }

  void generate(float phase) { sendOut(scale(taper(phase), startVoltage, level())); }

  auto isDeferring() const -> bool { return inputIsHigh(this, DeferInput) || buttonIsPressed(this, DeferButton); }

  auto isTriggered() const -> bool { return inputIsHigh(this, TriggerInput) || buttonIsPressed(this, TriggerButton); }

  auto level() const -> float { return selectableLevel(this, LevelKnob, LevelCvInput, LevelRangeSwitch); }

  void prepareToGenerate() { startVoltage = envelopeIn(); }

  auto taper(float input) const -> float {
    auto const curvature = dhe::curvature(this, CurveKnob, CurveCvInput);
    auto const taper = selectedTaper(this, ShapeSwitch);
    return taper->apply(input, curvature);
  }

  void sendActive() {
    auto const voltage = unipolarSignalRange.scale(isActive || activeButton());
    outputs[ActiveOutput].setVoltage(voltage);
  }

  void sendEoc() {
    auto const voltage = unipolarSignalRange.scale(isEoc || eocButton());
    outputs[EocOutput].setVoltage(voltage);
  }

  void setActive(bool active) { isActive = active; }

  void setEoc(bool eoc) { isEoc = eoc; }

  void sendOut(float voltage) { outputs[EnvelopeOutput].setVoltage(voltage); }

  StageStateMachine stateMachine{[this]() -> bool { return deferIsConnected(); },
                                 [this]() -> bool { return isDeferring(); },
                                 [this]() -> bool { return isTriggered(); },
                                 [this]() -> float { return duration(); },
                                 [this](float /*phase*/) { forward(); },
                                 [this]() { prepareToGenerate(); },
                                 [this](float phase) { generate(phase); },
                                 [this](bool active) { setActive(active); },
                                 [this](bool eoc) { setEoc(eoc); }};

  bool isActive{false};
  bool isEoc{false};
  float startVoltage{0.F};
};
} // namespace dhe
