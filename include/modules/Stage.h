#pragma once

#include "Module.h"

#include "controls/Duration.h"
#include "controls/Level.h"
#include "envelopes/StageStateMachine.h"

namespace DHE {

class Stage : public Module {
public:
  Stage();
  void process(const ProcessArgs &args) override;

  auto defer_gate_in() const -> bool;
  auto defer_gate_is_active() const -> bool;
  void forward();
  void generate(float phase);
  void prepare_to_generate();
  void set_active(bool active);
  void set_eoc(bool eoc);
  auto stage_gate_in() const -> bool;

  enum ParameterIIds { DURATION_KNOB, LEVEL_KNOB, CURVE_KNOB, PARAMETER_COUNT };

  enum InputIds { ENVELOPE_IN, STAGE_TRIGGER_IN, DEFER_GATE_IN, INPUT_COUNT };

  enum OutputIds { MAIN_OUT, EOC_OUT, ACTIVE_OUT, OUTPUT_COUNT };

private:
  auto curvature() const -> float;
  auto envelope_in() const -> float;
  void send_out(float voltage);
  auto taper(float phase) const -> float;

  Duration duration;
  Level level;
  StageStateMachine state_machine;
  float start_voltage{0.f};
};
} // namespace DHE
