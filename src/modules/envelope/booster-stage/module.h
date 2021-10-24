#pragma once

#include "signals.h"

#include "modules/envelope/mode/defer.h"
#include "modules/envelope/mode/generate.h"
#include "modules/envelope/mode/input.h"
#include "modules/envelope/mode/level.h"
#include "modules/envelope/stage/engine.h"
#include "params/curvature-config.h"
#include "params/duration-config.h"
#include "params/level-config.h"

#include "rack.hpp"

namespace dhe {
namespace envelope {
namespace booster_stage {
struct Module : public rack::engine::Module {
  Module() {
    config(Param::ParameterCount, Input::InputCount, Output::OutputCount);

    configInput(Input::EnvelopeInput, "Stage");
    configOutput(Output::EnvelopeOutput, "Stage");

    config_level_knob(this, Param::LevelKnob, Param::LevelRangeSwitch);
    config_level_range_switch(this, Param::LevelRangeSwitch);
    configInput(Input::LevelCvInput, "Level CV");

    config_curvature_knob(this, Param::CurveKnob);
    config_curve_shape_switch(this, Param::ShapeSwitch);
    configInput(Input::CurveCvInput, "Curvature CV");

    config_duration_knob(this, Param::DurationKnob, Param::DurationRangeSwitch);
    config_duration_range_switch(this, Param::DurationRangeSwitch);
    configInput(Input::DurationCvInput, "Duration CV");

    configInput(Input::DeferInput, "Defer");
    Button::config(this, Param::DeferButton, "Defer");
    configInput(Input::TriggerInput, "Trigger");
    Button::config(this, Param::TriggerButton, "Trigger");

    configOutput(Output::ActiveOutput, "Is active");
    Button::config(this, Param::ActiveButton, "Is active");
    configOutput(Output::EocOutput, "End of stage");
    Button::config(this, Param::EocButton, "End of stage");
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
