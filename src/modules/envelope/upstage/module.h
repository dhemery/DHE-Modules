#pragma once

#include "control-ids.h"
#include "engine.h"
#include "signals.h"

#include "params/level-config.h"
#include "params/presets.h"

#include "rack.hpp"

namespace dhe {
namespace envelope {
namespace upstage {

struct Module : public rack::engine::Module {
  Module() {
    config(Param::Count, Input::Count, Output::Count);

    config_level_knob(this, Param::Level, Param::LevelRange);
    config_level_range_switch(this, Param::LevelRange);
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