#pragma once

#include "control-ids.h"
#include "engine.h"
#include "modes.h"
#include "signals.h"

#include "controls/duration-controls.h"
#include "controls/knobs.h"
#include "modules/envelope/mode/defer.h"
#include "modules/envelope/mode/hold.h"
#include "modules/envelope/mode/idle.h"
#include "modules/envelope/mode/input.h"
#include "modules/envelope/mode/sustain.h"
#include "params/presets.h"
#include "signals/duration-signals.h"

#include "rack.hpp"

namespace dhe {
namespace envelope {
namespace hostage {

struct Module : public rack::engine::Module {
  Module() {
    config(ParamId::Count, InputId::Count, OutputId::Count);

    configInput(InputId::Envelope, "Stage");
    configInput(InputId::Defer, "Defer");
    configInput(InputId::Trigger, "Gate/Trigger");
    configOutput(OutputId::Active, "Is active");
    configOutput(OutputId::Eoc, "End of stage");
    configOutput(OutputId::Envelope, "Stage");

    auto *duration_knob =
        DurationKnob::config(this, ParamId::Duration, "Duration");
    auto *duration_range_switch = DurationRangeSwitch::config(
        this, ParamId::DurationRange, "Duration range");
    duration_range_switch->add_knob(duration_knob);
    configInput(InputId::DurationCv, "Duration CV");

    Switch::config<Modes>(this, ParamId::Mode, "Mode", Mode::Hold);
  };

  void process(ProcessArgs const &args) override {
    engine_.process(args.sampleTime);
  }

  auto dataToJson() -> json_t * override {
    auto *data = json_object();
    json_object_set_new(data, preset_version_key, json_integer(0));
    return data;
  }

private:
  using RackSignals =
      Signals<rack::engine::Param, rack::engine::Input, rack::engine::Output>;
  using DeferM = mode::DeferMode<RackSignals>;
  using HoldM = mode::HoldMode<RackSignals, PhaseTimer>;
  using IdleM = mode::IdleMode<RackSignals>;
  using InputM = mode::InputMode<RackSignals>;
  using SustainM = mode::SustainMode<RackSignals>;
  using RackEngine =
      Engine<RackSignals, InputM, DeferM, HoldM, SustainM, IdleM>;

  RackSignals controls_{params, inputs, outputs};
  DeferM defer_mode_{controls_};
  PhaseTimer hold_timer_{};
  HoldM hold_mode_{controls_, hold_timer_};
  IdleM idle_mode_{controls_};
  SustainM sustain_mode_{controls_};
  InputM input_mode_{controls_};
  RackEngine engine_{controls_,  input_mode_,   defer_mode_,
                     hold_mode_, sustain_mode_, idle_mode_};
};
} // namespace hostage
} // namespace envelope
} // namespace dhe
