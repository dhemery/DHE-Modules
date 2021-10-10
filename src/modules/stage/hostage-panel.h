#pragma once

#include "./hostage-controls.h"
#include "controls/ports.h"
#include "controls/switches.h"
#include "widgets/knobs.h"
#include "widgets/screws.h"

#include <app/ModuleWidget.hpp>

namespace dhe {
namespace stage {
class HostagePanel : public rack::app::ModuleWidget {
  using Controls = HostageControls;

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

    ThumbSwitch<2>::install(this, Controls::ModeSwitch, column2, y);

    y += dy;
    Input::install(this, Controls::DurationCvInput, column1, y);
    ThumbSwitch<3>::install(this, Controls::DurationRangeSwitch, column3, y);

    y += dy;
    Knob::install<Large>(this, Controls::DurationKnob, column2, y);

    y = 82.F;
    dy = 15.F;

    Input::install(this, Controls::DeferInput, column1, y);
    Output::install(this, Controls::ActiveOutput, column3, y);

    y += dy;
    Input::install(this, Controls::TriggerInput, column1, y);
    Output::install(this, Controls::EocOutput, column3, y);

    y += dy;
    Input::install(this, Controls::EnvelopeInput, column1, y);
    Output::install(this, Controls::EnvelopeOutput, column3, y);
  }
};
} // namespace stage
} // namespace dhe
