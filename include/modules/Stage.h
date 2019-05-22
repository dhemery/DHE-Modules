#pragma once

#include <functional>
#include <memory>

#include "Module.h"

#include "envelopes/StageStateMachine.h"
#include "modules/controls/CurvatureControl.h"

namespace dhe {

class Stage : public Module {
public:
  Stage();
  void process(const ProcessArgs &args) override;

  auto deferGateIn() -> bool;
  auto deferGateIsActive() const -> bool;
  void forward();
  void generate(float phase);
  void prepareToGenerate();
  void setActive(bool active);
  void setEoc(bool eoc);
  auto stageGateIn() -> bool;

  enum ParameterIds { DURATION_KNOB, LEVEL_KNOB, CURVE_KNOB, PARAMETER_COUNT };

  enum InputIds { ENVELOPE_IN, STAGE_TRIGGER_IN, DEFER_GATE_IN, INPUT_COUNT };

  enum OutputIds { MAIN_OUT, EOC_OUT, ACTIVE_OUT, OUTPUT_COUNT };

private:
  auto envelopeIn() -> float;
  void sendOut(float voltage);

  std::function<float()> duration;
  std::function<float()> level;
  std::unique_ptr<CurvatureControl> shape;
  StageStateMachine stateMachine;
  float startVoltage{0.f};
};
} // namespace dhe
