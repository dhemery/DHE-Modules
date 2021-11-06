#pragma once

#include "control-ids.h"
#include "engine.h"
#include "signals.h"

#include "controls/knob-styles.h"
#include "controls/knobs.h"
#include "modules/envelope/mode/defer.h"
#include "modules/envelope/mode/generate.h"
#include "modules/envelope/mode/input.h"
#include "modules/envelope/mode/level.h"
#include "params/curvature-config.h"
#include "params/duration-config.h"

#include "rack.hpp"

namespace dhe {
namespace envelope {
namespace stage {
struct Module : public rack::engine::Module {
  Module() {
    config(Param::Count, Input::Count, Output::Count);

    config_duration_knob(this, Param::Duration, medium_duration_range);
    Knob::config<Unipolar>(this, Param::Level, "Level", 5.F);
    config_curvature_knob(this, Param::Curvature);

    configInput(Input::Envelope, "Stage");
    configInput(Input::Defer, "Defer");
    configInput(Input::Trigger, "Trigger");
    configOutput(Output::Active, "Is active");
    configOutput(Output::Eoc, "End of stage");
    configOutput(Output::Envelope, "Stage");
  }

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
  using GenerateM = mode::GenerateMode<RackSignals, PhaseTimer>;
  using InputM = mode::InputMode<RackSignals>;
  using LevelM = mode::LevelMode<RackSignals>;
  using RackEngine = Engine<RackSignals, DeferM, InputM, GenerateM, LevelM>;

  RackSignals signals_{params, inputs, outputs};
  PhaseTimer timer_{};
  DeferM defer_mode_{signals_};
  InputM input_mode_{signals_};
  GenerateM generate_mode_{signals_, timer_};
  LevelM level_mode_{signals_};
  RackEngine engine_{signals_, defer_mode_, input_mode_, generate_mode_,
                     level_mode_};
};
} // namespace stage
} // namespace envelope
} // namespace dhe
