#pragma once

#include "Module.h"

#include "controls/Duration.h"
#include "envelopes/HostageStateMachine.h"
#include "util/selector.h"

namespace DHE {

class Hostage : public Module {
public:
  Hostage();
  void process(const ProcessArgs &args) override;

  auto defer_gate_in() const -> bool;
  auto defer_gate_is_active() const -> bool;
  void forward();
  auto is_sustain_mode() const -> bool;
  void set_active(bool active);
  void set_eoc(bool eoc);
  auto stage_gate_in() const -> bool;

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
  auto envelope_in() const -> float;
  void send_out(float voltage);

  Duration duration;
  HostageStateMachine state_machine;
};
} // namespace DHE
