#pragma once

#include <functional>

#include "Module.h"

#include "envelopes/HostageStateMachine.h"

namespace dhe {

class Hostage : public Module {
public:
  Hostage();
  void process(const ProcessArgs &args) override;

  auto deferGateIn() -> bool;
  auto deferGateIsActive() const -> bool;
  void forward();
  auto isSustainMode() -> bool;
  void setActive(bool active);
  void setEoc(bool eoc);
  auto stageGateIn() -> bool;

  enum InputIds {
    DEFER_GATE_IN,
    DURATION_CV,
    MAIN_IN,
    STAGE_GATE_IN,
    INPUT_COUNT
  };

  enum OutputIds { ACTIVE_OUT, MAIN_OUT, EOC_OUT, OUTPUT_COUNT };

  enum ParameterIds {
    DURATION_KNOB,
    DURATION_RANGE_SWITCH,
    HOSTAGE_MODE_SWITCH,
    PARAMETER_COUNT
  };

private:
  auto envelopeIn() -> float;
  void sendOut(float voltage);

  std::function<float()> duration;
  HostageStateMachine stateMachine;
};
} // namespace dhe
