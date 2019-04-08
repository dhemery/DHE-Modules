#pragma once

#include "modules/Module.h"

#include "envelopes/hostage-state-machine.h"
#include "util/duration.h"

namespace DHE {

class Hostage : public Module {
public:
  Hostage();
  void step() override;
  auto defer_gate_in() const -> bool;
  auto defer_gate_is_active() const -> bool;
  auto duration() const -> float;
  void forward();
  auto is_sustain_mode() const -> bool;
  auto sample_time() const -> float;
  ;
  void set_active(bool active);
  void set_eoc(bool eoc);
  auto stage_gate_in() const -> bool;

  const Selector<Range const *> duration_range_selector{
      Duration::ranges, [this](Range const *range) { duration_range = range; }};

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

  HostageStateMachine<Hostage> state_machine{this};
  Range const *duration_range{&Duration::medium_range};
};
} // namespace DHE
