#pragma once

#include <memory>

#include "Module.h"

#include "envelopes/StageStateMachine.h"
#include "modules/controls/CurvatureControl.h"
#include "modules/controls/DurationControl.h"
#include "modules/controls/LevelControl.h"
#include "util/sigmoid.h"

namespace DHE {

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

  void setDurationRange(float switchPosition) {
    durationRange = Duration::range(switchPosition);
  }
  enum ParameterIds {
    ACTIVE_BUTTON,
    CURVE_KNOB,
    DEFER_BUTTON,
    DURATION_KNOB,
    DURATION_RANGE_SWITCH,
    EOC_BUTTON,
    LEVEL_KNOB,
    LEVEL_RANGE_SWITCH,
    SHAPE_SWITCH,
    TRIGGER_BUTTON,
    PARAMETER_COUNT
  };

  enum InputIds {
    CURVE_CV,
    DEFER_GATE_IN,
    DURATION_CV,
    LEVEL_CV,
    ENVELOPE_IN,
    STAGE_TRIGGER_IN,
    INPUT_COUNT
  };

  enum OutputIds { ACTIVE_OUT, EOC_OUT, MAIN_OUT, OUTPUT_COUNT };

private:
  auto envelopeIn() -> float;
  void sendActive();
  void sendEoc();
  void sendOut(float voltage);

  StageStateMachine stateMachine;
  bool isActive{false};
  bool isEoc{false};
  float startVoltage{0.f};
  Range const *durationRange;
  std::unique_ptr<Duration::Control> duration;
  std::unique_ptr<LevelControl> level;
  std::unique_ptr<CurvatureControl> shape;
};
} // namespace DHE
