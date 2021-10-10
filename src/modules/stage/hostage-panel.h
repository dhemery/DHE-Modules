#pragma once

#include "./hostage-controls.h"
#include "controls/port.h"
#include "widgets/knobs.h"
#include "widgets/screws.h"
#include "widgets/switches.h"

#include <app/ModuleWidget.hpp>

namespace dhe {
namespace stage {
class HostagePanel : public rack::app::ModuleWidget {
  using Controls = HostageControls;
  using Knob = Knobs<HostagePanel>;
  using Switch = Switches<HostagePanel>;

public:
  static auto constexpr svg_dir = "hostage";

  HostagePanel(rack::engine::Module *module) {
    auto constexpr hp = 5;

    setModule(module);
    setPanel(load_svg(svg_dir, "hostage"));
    install_screws(this, hp);

    auto constexpr width = hp2mm(hp);

    auto constexpr column1 = width / 4.F + 0.333333F;
    auto constexpr column2 = width / 2.F;
    auto constexpr column3 = width - column1;

    auto y = 25.F;
    auto dy = 18.5F;

    addParam(Switch::thumb<2>(module, column2, y, Controls::ModeSwitch));

    y += dy;
    Input::install(this, y, Controls::DurationCvInput, column1);
    addParam(
        Switch::thumb<3>(module, column3, y, Controls::DurationRangeSwitch));

    y += dy;
    addParam(Knob::large(module, column2, y, Controls::DurationKnob));

    y = 82.F;
    dy = 15.F;

    Input::install(this, y, Controls::DeferInput, column1);
    Output::install(this, y, Controls::ActiveOutput, column3);

    y += dy;
    Input::install(this, y, column1, Controls::TriggerInput);
    Output::install(this, y, Controls::EocOutput, column3);

    y += dy;
    Input::install(this, y, Controls::EnvelopeInput, column1);
    Output::install(this, y, Controls::EnvelopeOutput, column3);
  }
};
} // namespace stage
} // namespace dhe
