#pragma once

#include "./upstage-controls.h"
#include "controls/port.h"
#include "widgets/knobs.h"
#include "widgets/screws.h"
#include "widgets/switches.h"

#include <app/ModuleWidget.hpp>

namespace dhe {
namespace stage {
class UpstagePanel : public rack::app::ModuleWidget {
  using Controls = UpstageControls;
  using Switch = Switches<UpstagePanel>;

public:
  static auto constexpr svg_dir = "upstage";

  UpstagePanel(rack::engine::Module *module) {
    auto constexpr hp = 5;

    setModule(module);
    setPanel(load_svg(svg_dir, "upstage"));
    install_screws(this, hp);

    auto constexpr width = hp2mm(hp);

    auto constexpr column1 = width / 4.F + 0.333333333F;
    auto constexpr column2 = width / 2.F;
    auto constexpr column3 = width - column1;

    auto y = 25.F;
    auto dy = 18.5F;

    Knob::install<Large>(this, Controls::LevelKnob, column2, y);

    y += dy;
    Input::install(this, Controls::LevelCvInput, column1, y);
    ThumbSwitch<2>::install(this, Controls::LevelRangeSwitch, column3, y);

    y += dy;
    addParam(Switch::momentary(module, column1, y, Controls::WaitButton));
    addParam(Switch::momentary(module, column3, y, Controls::TriggerButton));

    y = 82.F;
    dy = 15.F;

    Input::install(this, Controls::WaitInput, column1, y);

    y += dy;
    Input::install(this, Controls::TriggerInput, column1, y);
    Output::install(this, Controls::TriggerOutput, column3, y);

    y += dy;
    Output::install(this, Controls::EnvelopeOutput, column3, y);
  }
};
} // namespace stage

} // namespace dhe
