#pragma once

#include <functional>
#include <memory>

#include "Module.h"

#include "envelopes/HostageStateMachine.h"
#include "modules/controls/Duration.h"

namespace DHE {

class Hostage : public Module {
public:
  Hostage();
  void process(const ProcessArgs &args) override;

  auto defer_gate_in() -> bool;
  auto defer_gate_is_active() const -> bool;
  void forward();
  auto is_sustain_mode() -> bool;
  void set_active(bool active);
  void set_eoc(bool eoc);
  auto stage_gate_in() -> bool;

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
  auto envelope_in() -> float;
  void send_out(float voltage);

  std::function<float()> duration;
  Range const *durationRange{&Duration::mediumRange};
  HostageStateMachine state_machine;
};
} // namespace DHE
