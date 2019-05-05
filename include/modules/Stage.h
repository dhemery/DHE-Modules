#pragma once

#include <memory>

#include "Module.h"

#include "controls/Duration.h"
#include "controls/Level.h"
#include "envelopes/StageStateMachine.h"

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
  auto curvature() -> float;
  auto envelope_in() -> float;
  void send_out(float voltage);
  auto taper(float phase) -> float;

  std::unique_ptr<Duration> duration;
  std::unique_ptr<Level> level;
  StageStateMachine state_machine;
  float start_voltage{0.f};
};
} // namespace DHE
