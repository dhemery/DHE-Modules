#pragma once

#include "controls.h"
#include "engine.h"

#include "controls/knobs.h"
#include "modules/envelope/mode/defer.h"
#include "modules/envelope/mode/hold.h"
#include "modules/envelope/mode/idle.h"
#include "modules/envelope/mode/input.h"
#include "modules/envelope/mode/sustain.h"
#include "params/duration-config.h"

#include "rack.hpp"

namespace dhe {
namespace envelope {
namespace hostage {

class Module : public rack::engine::Module {
  using Controls = HostageControls;
  using DeferM = mode::DeferMode<Controls>;
  using HoldM = mode::HoldMode<Controls, PhaseTimer>;
  using IdleM = mode::IdleMode<Controls>;
  using InputM = mode::InputMode<Controls>;
  using SustainM = mode::SustainMode<Controls>;

public:
  Module() {
    config(Controls::ParameterCount, Controls::InputCount,
           Controls::OutputCount);

    configInput(Controls::EnvelopeInput, "Stage");
    configInput(Controls::DeferInput, "Defer");
    configInput(Controls::TriggerInput, "Gate/Trigger");
    configOutput(Controls::ActiveOutput, "Is active");
    configOutput(Controls::EocOutput, "End of stage");
    configOutput(Controls::EnvelopeOutput, "Stage");

    config_duration_knob(this, Controls::DurationKnob,
                         Controls::DurationRangeSwitch);
    config_duration_range_switch(this, Controls::DurationRangeSwitch);
    configInput(Controls::DurationCvInput, "Duration CV");

    Switch::config(this, Controls::ModeSwitch, "Mode", {"Hold", "Sustain"}, 0);
  };

  void process(ProcessArgs const &args) override {
    machine_.process(args.sampleTime);
  }

  auto dataToJson() -> json_t * override {
    auto *data = json_object();
    json_object_set_new(data, preset_version_key, json_integer(0));
    return data;
  }

private:
  Controls controls_{inputs, params, outputs};
  DeferM defer_mode_{controls_};
  PhaseTimer hold_timer_{};
  HoldM hold_mode_{controls_, hold_timer_};
  IdleM idle_mode_{controls_};
  SustainM sustain_mode_{controls_};
  InputM input_mode_{controls_};
  HostageEngine<Controls, InputM, DeferM, HoldM, SustainM, IdleM> machine_{
      controls_,  input_mode_,   defer_mode_,
      hold_mode_, sustain_mode_, idle_mode_};
};
} // namespace hostage
} // namespace envelope
} // namespace dhe
