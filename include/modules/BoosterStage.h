#pragma once
#include "Module.h"
#include "envelopes/StageStateMachine.h"

#include <functional>

namespace dhe {

class BoosterStage : public Module {
public:
  BoosterStage();
  void process(const ProcessArgs &args) override;

  auto deferGateIn() -> bool;
  auto deferGateIsActive() const -> bool;
  void forward();
  void generate(float phase);
  void prepareToGenerate();
  void setActive(bool active);
  void setEoc(bool eoc);
  auto stageGateIn() -> bool;

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

  enum InputIds {
    CurveCvInput,
    DeferGateInput,
    DurationCvInput,
    LevelCvInput,
    EnvelopeInput,
    TriggerInput,
    InputCount
  };

  enum OutputIds { ActiveOutput, EocOutput, EnvelopeOutput, OutputCount };

private:
  auto envelopeIn() -> float;
  void sendActive();
  void sendEoc();
  void sendOut(float voltage);

  StageStateMachine stateMachine;
  bool isActive{false};
  bool isEoc{false};
  float startVoltage{0.F};
  std::function<float()> duration;
  std::function<float()> level;
  std::function<float(float)> taper;
};
} // namespace dhe
