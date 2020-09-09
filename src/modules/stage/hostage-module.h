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
  using DeferM = DeferMode<Controls>;
  using HoldM = HoldMode<Controls, PhaseTimer>;
  using IdleM = IdleMode<Controls>;
  using InputM = InputMode<Controls>;
  using SustainM = SustainMode<Controls>;

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
} // namespace stage
} // namespace dhe
