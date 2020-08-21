#pragma once

#include "DeferMode.h"
#include "HoldMode.h"
#include "HostageControls.h"
#include "HostageEngine.h"
#include "IdleMode.h"
#include "InputMode.h"
#include "SustainMode.h"
#include "config/duration-config.h"

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
    machine.process(args.sampleTime);
  }

private:
  Controls controls{inputs, params, outputs};
  DeferMode<Controls> deferMode{controls};
  PhaseTimer holdTimer{};
  HoldMode<Controls> holdMode{controls, holdTimer};
  IdleMode<Controls> idleMode{controls};
  SustainMode<Controls> sustainMode{controls};
  InputMode<Controls> inputMode{controls};
  HostageEngine<Controls, InputMode<Controls>, DeferMode<Controls>,
                HoldMode<Controls>, SustainMode<Controls>, IdleMode<Controls>>
      machine{controls, inputMode, deferMode, holdMode, sustainMode, idleMode};
};
} // namespace stage
} // namespace dhe
