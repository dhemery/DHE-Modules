#pragma once

#include "control-ids.h"
#include "engine.h"
#include "signals.h"

#include "controls/duration-controls.h"
#include "controls/knobs.h"
#include "controls/shape-controls.h"
#include "controls/voltage-controls.h"
#include "modules/envelope/mode/defer-mode.h"
#include "modules/envelope/mode/generate-mode.h"
#include "modules/envelope/mode/input-mode.h"
#include "modules/envelope/mode/level-mode.h"
#include "params/presets.h"

#include "rack.hpp"

namespace dhe {
namespace envelope {
namespace stage {
struct Module : public rack::engine::Module {
  Module() {
    config(ParamId::Count, InputId::Count, OutputId::Count);

    MediumDurationKnob::config(this, ParamId::Duration, "Duration");
    UnipolarKnob::config(this, ParamId::Level, "Level");
    CurvatureKnob::config(this, ParamId::Curvature, "Curvature");

    configInput(InputId::Envelope, "Stage");
    configInput(InputId::Defer, "Defer");
    configInput(InputId::Trigger, "Trigger");
    configOutput(OutputId::Active, "Is active");
    configOutput(OutputId::Eoc, "End of stage");
    configOutput(OutputId::Envelope, "Stage");
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
  using DeferM = DeferMode<RackSignals>;
  using GenerateM = GenerateMode<RackSignals, PhaseTimer>;
  using InputM = InputMode<RackSignals>;
  using LevelM = LevelMode<RackSignals>;
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
