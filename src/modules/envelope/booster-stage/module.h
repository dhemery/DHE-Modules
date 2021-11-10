#pragma once

#include "signals.h"

#include "controls/knobs.h"
#include "controls/switches.h"
#include "modules/envelope/mode/defer.h"
#include "modules/envelope/mode/generate.h"
#include "modules/envelope/mode/input.h"
#include "modules/envelope/mode/level.h"
#include "modules/envelope/stage/engine.h"
#include "signals/durations.h"
#include "signals/shapes.h"
#include "signals/voltages.h"

#include "rack.hpp"

namespace dhe {
namespace envelope {
namespace booster_stage {
struct Module : public rack::engine::Module {
  Module() {
    config(Param::Count, Input::Count, Output::Count);

    configInput(Input::Envelope, "Stage");
    configOutput(Output::Envelope, "Stage");

    auto *level_knob =
        Knob::config<UnipolarVoltage>(this, Param::Level, "Level");
    auto const update_level_knob_range = [level_knob](Range r) {
      level_knob->set_display_range(r);
    };
    Picker::config<Voltages>(this, Param::LevelRange, "Level Range",
                             Voltages::Unipolar)
        ->on_change(update_level_knob_range);

    configInput(Input::LevelCv, "Level CV");

    CustomKnob::config<Curvature>(this, Param::Curvature, "Curvature");
    Switch::config<Shapes>(this, Param::Shape, "Shape", Shapes::J);
    configInput(Input::CurvatureCv, "Curvature CV");

    CustomKnob::config<Durations>(this, Param::Duration, "Duration");
    Switch::config<Durations>(this, Param::DurationRange, "Duration range",
                              Durations::Medium);
    configInput(Input::DurationCv, "Duration CV");

    configInput(Input::Defer, "Defer");
    Button::config(this, Param::Defer, "Defer");
    configInput(Input::Trigger, "Trigger");
    Button::config(this, Param::Trigger, "Trigger");

    configOutput(Output::Active, "Is active");
    Button::config(this, Param::Active, "Is active");
    configOutput(Output::Eoc, "End of stage");
    Button::config(this, Param::Eoc, "End of stage");
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
  using DeferMode = envelope::mode::DeferMode<RackSignals>;
  using InputMode = envelope::mode::InputMode<RackSignals>;
  using GenerateMode = envelope::mode::GenerateMode<RackSignals, PhaseTimer>;
  using LevelMode = envelope::mode::LevelMode<RackSignals>;
  using RackEngine =
      stage::Engine<RackSignals, DeferMode, InputMode, GenerateMode, LevelMode>;

  RackSignals signals_{params, inputs, outputs};
  PhaseTimer timer_{};
  DeferMode defer_mode_{signals_};
  InputMode input_mode_{signals_};
  GenerateMode generate_mode_{signals_, timer_};
  LevelMode level_mode_{signals_};
  RackEngine engine_{signals_, defer_mode_, input_mode_, generate_mode_,
                     level_mode_};
};
} // namespace booster_stage
} // namespace envelope
} // namespace dhe
