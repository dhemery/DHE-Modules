#pragma once

#include "Module.h"

#include "envelopes/StageStateMachine.h"

namespace DHE {

class Stage : public Module {
public:
  Stage() : Stage{[]() -> float { return rack::engineGetSampleTime(); }} {}
  explicit Stage(const std::function<float()> &sample_time);

  auto defer_gate_in() const -> bool;
  auto defer_gate_is_active() const -> bool;
  auto duration() const -> float;
  void forward();
  void generate(float phase);
  void prepare_to_generate();
  void set_active(bool active);
  void set_eoc(bool eoc);
  auto stage_gate_in() const -> bool;
  void step() override;

  enum ParameterIIds { DURATION_KNOB, LEVEL_KNOB, CURVE_KNOB, PARAMETER_COUNT };

  enum InputIds { ENVELOPE_IN, STAGE_TRIGGER_IN, DEFER_GATE_IN, INPUT_COUNT };

  enum OutputIds { MAIN_OUT, EOC_OUT, ACTIVE_OUT, OUTPUT_COUNT };

private:
  auto curvature() const -> float;
  auto envelope_in() const -> float;
  auto level() const -> float;
  void send_out(float voltage);
  auto taper(float phase) const -> float;

  StageStateMachine state_machine;
  float start_voltage{0.f};
};
} // namespace DHE
