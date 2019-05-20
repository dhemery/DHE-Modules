#pragma once

#include <memory>

#include "Module.h"

#include "envelopes/StageStateMachine.h"
#include "modules/controls/CurvatureControl.h"
#include "modules/controls/Duration.h"
#include "modules/controls/LevelControl.h"

namespace DHE {

class Stage : public Module {
public:
  Stage();
  void process(const ProcessArgs &args) override;

  auto defer_gate_in() -> bool;
  auto defer_gate_is_active() const -> bool;
  void forward();
  void generate(float phase);
  void prepare_to_generate();
  void set_active(bool active);
  void set_eoc(bool eoc);
  auto stage_gate_in() -> bool;

  enum ParameterIIds { DURATION_KNOB, LEVEL_KNOB, CURVE_KNOB, PARAMETER_COUNT };

  enum InputIds { ENVELOPE_IN, STAGE_TRIGGER_IN, DEFER_GATE_IN, INPUT_COUNT };

  enum OutputIds { MAIN_OUT, EOC_OUT, ACTIVE_OUT, OUTPUT_COUNT };

private:
  auto envelope_in() -> float;
  void send_out(float voltage);

  std::unique_ptr<Duration::Control> duration;
  std::unique_ptr<LevelControl> level;
  std::unique_ptr<CurvatureControl> shape;
  StageStateMachine state_machine;
  float start_voltage{0.f};
};
} // namespace DHE
