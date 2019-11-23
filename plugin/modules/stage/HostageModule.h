#pragma once

#include "Hostage.h"
#include "HostageControls.h"
#include "config/DurationConfig.h"

#include <engine/Module.hpp>

namespace dhe {
namespace stage {

  class HostageModule : public rack::engine::Module {
    using Controls = HostageControls;

  public:
    HostageModule() : rack::engine::Module{} {
      config(Controls::ParameterCount, Controls::InputCount, Controls::OutputCount);

      configDurationKnob(this, Controls::DurationKnob, Controls::DurationRangeSwitch);
      configDurationRangeSwitch(this, Controls::DurationRangeSwitch);

      configToggle<2>(this, Controls::ModeSwitch, "Mode", {"Hold", "Sustain"}, 0);
    };

    void process(const ProcessArgs &args) override { machine.process(args.sampleTime); }

  private:
    Controls controls{inputs, params, outputs};
    Hostage<Controls> machine{controls};
  };
} // namespace stage
} // namespace dhe
