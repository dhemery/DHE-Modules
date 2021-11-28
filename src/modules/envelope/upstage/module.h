#pragma once

#include "control-ids.h"
#include "engine.h"
#include "signals.h"

#include "controls/buttons.h"
#include "controls/knobs.h"
#include "controls/switches.h"
#include "controls/voltage-controls.h"
#include "params/presets.h"
#include "signals/voltage-signals.h"

#include "rack.hpp"

namespace dhe {
namespace envelope {
namespace upstage {

struct Module : public rack::engine::Module {
  Module() {
    config(ParamId::Count, InputId::Count, OutputId::Count);

    auto *level_knob = VoltageKnob::config(this, ParamId::Level, "Level");
    VoltageRangeSwitch::config(this, ParamId::LevelRange, "Level range",
                               VoltageRangeId::Unipolar)
        ->add_knob(level_knob);
    configInput(InputId::LevelCv, "Level CV");

    configInput(InputId::Trigger, "Trigger");
    Button::config(this, ParamId::Trigger, "Trigger");

    configInput(InputId::Wait, "Wait");
    Button::config(this, ParamId::Wait, "Wait");

    configOutput(OutputId::Trigger, "Trigger");
    configOutput(OutputId::Envelope, "Stage");
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
