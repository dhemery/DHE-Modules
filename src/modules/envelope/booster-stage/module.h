#pragma once

#include "signals.h"

#include "modules/envelope/mode/defer-mode.h"
#include "modules/envelope/mode/generate-mode.h"
#include "modules/envelope/mode/input-mode.h"
#include "modules/envelope/mode/level-mode.h"
#include "modules/envelope/stage/engine.h"

#include "controls/buttons.h"
#include "controls/duration-controls.h"
#include "controls/shape-controls.h"
#include "controls/voltage-controls.h"
#include "params/presets.h"
#include "signals/duration-signals.h"
#include "signals/shape-signals.h"
#include "signals/voltage-signals.h"

#include "rack.hpp"

namespace dhe {
namespace envelope {
namespace booster_stage {
struct Module : public rack::engine::Module {
  Module() {
    config(ParamId::Count, InputId::Count, OutputId::Count);

    configInput(InputId::Envelope, "Stage");
    configOutput(OutputId::Envelope, "Stage");

    auto *level_knob = VoltageKnob::config(this, ParamId::Level, "Level");
    VoltageRangeSwitch::config(this, ParamId::LevelRange, "Level Range",
                               VoltageRangeId::Unipolar)
        ->add_knob(level_knob);

    configInput(InputId::LevelCv, "Level CV");

    CurvatureKnob::config(this, ParamId::Curvature, "Curvature");
    ShapeSwitch::config(this, ParamId::Shape, "Shape", Shape::Id::J);
    configInput(InputId::CurvatureCv, "Curvature CV");

    auto *duration_range_switch =
        DurationRangeSwitch::config(this, ParamId::DurationRange,
                                    "Duration range", DurationRangeId::Medium);
    auto *duration_knob =
        DurationKnob::config(this, ParamId::Duration, "Duration");
    duration_range_switch->add_knob(duration_knob);
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
  using DeferMode = envelope::DeferMode<RackSignals>;
  using InputMode = envelope::InputMode<RackSignals>;
  using GenerateMode = envelope::GenerateMode<RackSignals, PhaseTimer>;
  using LevelMode = envelope::LevelMode<RackSignals>;
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
