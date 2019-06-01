#pragma once
#include "Module.h"
#include "envelopes/StageStateMachine.h"
#include "modules/components/Taper.h"
#include "modules/controls/Controls.h"
#include "modules/controls/CurvatureControls.h"
#include "modules/controls/Duration.h"
#include "modules/controls/Inputs.h"
#include "modules/controls/Level.h"
#include "modules/controls/TaperControls.h"

#include <functional>

namespace dhe {

class BoosterStage : public Module {
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
  auto envelopeIn() -> float { return inputs[EnvelopeInput].getVoltage(); }
  auto activeButton() -> bool { return isPressed(this, ActiveButton); }
  auto eocButton() -> bool { return isPressed(this, EocButton); }

  auto level() -> float { return scaled<2>(this, LevelKnob, LevelCvInput, LevelRangeSwitch, level::ranges); }

  auto taper(float input) -> float {
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
      input::isConnected(this, DeferInput),
      input::isHigh(this, DeferInput, DeferButton),
      input::isHigh(this, TriggerInput, TriggerButton),
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
