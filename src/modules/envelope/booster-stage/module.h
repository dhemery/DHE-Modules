#pragma once

#include "signals.h"

#include "modules/envelope/mode/defer.h"
#include "modules/envelope/mode/generate.h"
#include "modules/envelope/mode/input.h"
#include "modules/envelope/mode/level.h"
#include "modules/envelope/stage/engine.h"

#include "controls/buttons.h"
#include "controls/knobs.h"
#include "controls/switches.h"
#include "params/duration-params.h"
#include "params/presets.h"
#include "signals/curvature.h"
#include "signals/duration-signals.h"
#include "signals/voltages.h"

#include "rack.hpp"

namespace dhe {
namespace envelope {
namespace booster_stage {
struct Module : public rack::engine::Module {
  Module() {
    config(ParamId::Count, InputId::Count, OutputId::Count);

    configInput(InputId::Envelope, "Stage");
    configOutput(OutputId::Envelope, "Stage");

    auto *level_knob = Knob::config<Voltage>(this, ParamId::Level, "Level");
    auto const select_level_range = [level_knob](VoltageRangeId id) {
      level_knob->mapper().select_range(id);
    };
    Switch::config<VoltageRanges>(this, ParamId::LevelRange, "Level Range",
                                  VoltageRangeId::Unipolar)
        ->on_change(select_level_range);

    configInput(InputId::LevelCv, "Level CV");

    Knob::config<Curvature>(this, ParamId::Curvature, "Curvature");
    Switch::config<Shapes>(this, ParamId::Shape, "Shape", Shape::Id::J);
    configInput(InputId::CurvatureCv, "Curvature CV");

    auto *duration_knob =
        DurationKnob::config(this, ParamId::Duration, "Duration");
    auto select_duration_range = [duration_knob](DurationRangeId range) {
      duration_knob->select_range(range);
    };
    Switch::config<DurationRanges>(this, ParamId::DurationRange,
                                   "Duration range", DurationRangeId::Medium)
        ->on_change(select_duration_range);
    configInput(InputId::DurationCv, "Duration CV");

    configInput(InputId::Defer, "Defer");
    Button::config(this, ParamId::Defer, "Defer");
    configInput(InputId::Trigger, "Trigger");
    Button::config(this, ParamId::Trigger, "Trigger");

    configOutput(OutputId::Active, "Is active");
    Button::config(this, ParamId::Active, "Is active");
    configOutput(OutputId::Eoc, "End of stage");
    Button::config(this, ParamId::Eoc, "End of stage");
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
