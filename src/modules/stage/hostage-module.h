#pragma once

#include "config/duration-config.h"
#include "defer-mode.h"
#include "hold-mode.h"
#include "hostage-controls.h"
#include "hostage-engine.h"
#include "idle-mode.h"
#include "input-mode.h"
#include "sustain-mode.h"

#include <engine/Module.hpp>

namespace dhe {
namespace stage {

class HostageModule : public rack::engine::Module {
  using Controls = HostageControls;

public:
  HostageModule() {
    config(Controls::ParameterCount, Controls::InputCount,
           Controls::OutputCount);

    config_duration_knob(this, Controls::DurationKnob,
                         Controls::DurationRangeSwitch);
    config_duration_range_switch(this, Controls::DurationRangeSwitch);

    config_toggle<2>(this, Controls::ModeSwitch, "Mode", {"Hold", "Sustain"},
                     0);
  };

  void process(ProcessArgs const &args) override {
    machine_.process(args.sampleTime);
  }

private:
  Controls controls_{inputs, params, outputs};
  DeferMode<Controls> defer_mode_{controls_};
  PhaseTimer hold_timer_{};
  HoldMode<Controls> hold_mode_{controls_, hold_timer_};
  IdleMode<Controls> idle_mode_{controls_};
  SustainMode<Controls> sustain_mode_{controls_};
  InputMode<Controls> input_mode_{controls_};
  HostageEngine<Controls, InputMode<Controls>, DeferMode<Controls>,
                HoldMode<Controls>, SustainMode<Controls>, IdleMode<Controls>>
      machine_{controls_,  input_mode_,   defer_mode_,
               hold_mode_, sustain_mode_, idle_mode_};
};
} // namespace stage
} // namespace dhe
