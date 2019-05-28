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
  auto deferGateIsActive() const -> bool;
  auto envelopeIn() -> float;
  void forward();
  void generate(float phase);
  void prepareToGenerate();
  void sendActive();
  void sendEoc();
  void sendOut(float voltage);
  void setActive(bool active);
  void setEoc(bool eoc);

  std::function<bool()> activeButton{button::state(this, ActiveButton)};
  std::function<bool()> deferIsConnected{input::isConnected(this, DeferInput)};
  std::function<bool()> deferButton{button::state(this, DeferButton)};
  std::function<bool()> deferInput{input::isHigh(this, DeferInput)};
  std::function<float()> duration{
      duration::withSelectableRange(this, DurationKnob, DurationCvInput, DurationRangeSwitch)};
  std::function<bool()> eocButton{button::state(this, EocButton)};
  std::function<float()> level{level::withSelectableRange(this, LevelKnob, LevelCvInput, LevelRangeSwitch)};

  std::function<bool()> triggerButton{button::state(this, TriggerButton)};
  std::function<bool()> triggerInput{input::isHigh(this, TriggerInput)};
  std::function<float(float)> taper{taper::withSelectableShape(this, CurveKnob, CurveCvInput, ShapeSwitch)};

  StageStateMachine stateMachine;
  bool isActive{false};
  bool isEoc{false};
  float startVoltage{0.F};
};
} // namespace dhe
