#pragma once
#include "Module.h"
#include "envelopes/StageStateMachine.h"
#include "modules/controls/Controls.h"
#include "modules/controls/Curvature.h"
#include "modules/controls/Duration.h"
#include "modules/controls/Level.h"

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
  auto envelopeIn() -> float;
  void forward();
  void generate(float phase);
  void prepareToGenerate();
  void sendActive();
  void sendEoc();
  void sendOut(float voltage);
  void setActive(bool active);
  void setEoc(bool eoc);

  std::function<bool()> const activeButton{button::state(this, ActiveButton)};
  std::function<bool()> const eocButton{button::state(this, EocButton)};
  std::function<float()> const level{level::withSelectableRange(this, LevelKnob, LevelCvInput, LevelRangeSwitch)};
  std::function<float(float)> const taper{taper::withSelectableShape(this, CurveKnob, CurveCvInput, ShapeSwitch)};

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
