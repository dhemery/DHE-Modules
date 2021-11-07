#pragma once

#include "control-ids.h"
#include "engine.h"
#include "signals.h"

#include "controls/knobs.h"
#include "controls/switches.h"
#include "params/presets.h"
#include "signals/levels.h"

#include "rack.hpp"

namespace dhe {
namespace envelope {
namespace upstage {

struct Module : public rack::engine::Module {
  Module() {
    config(Param::Count, Input::Count, Output::Count);

    auto *level_knob = Knob::config<Unipolar>(this, Param::Level, "Level", 5.F);
    auto update_level_knob_range = [level_knob](Range r) {
      level_knob->set_display_range(r);
    };
    Picker::config<Levels>(this, Param::LevelRange, "Level range",
                           Levels::Unipolar)
        ->on_change(update_level_knob_range);
    configInput(Input::LevelCv, "Level CV");

    configInput(Input::Trigger, "Trigger");
    Button::config(this, Param::Trigger, "Trigger");

    configInput(Input::Wait, "Wait");
    Button::config(this, Param::Wait, "Wait");

    configOutput(Output::Trigger, "Trigger");
    configOutput(Output::Envelope, "Stage");
  }

  void process(ProcessArgs const & /*args*/) override { engine_.process(); }

  auto dataToJson() -> json_t * override {
    auto *data = json_object();
    json_object_set_new(data, preset_version_key, json_integer(0));
    return data;
  }

private:
  using RackSignals =
      Signals<rack::engine::Param, rack::engine::Input, rack::engine::Output>;
  using RackEngine = Engine<RackSignals>;

  RackSignals signals_{params, inputs, outputs};
  RackEngine engine_{signals_};
};
} // namespace upstage
} // namespace envelope
} // namespace dhe
