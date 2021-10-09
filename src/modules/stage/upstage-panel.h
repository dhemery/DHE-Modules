#pragma once

#include "./upstage-controls.h"
#include "widgets/knobs.h"
#include "widgets/ports.h"
#include "widgets/screws.h"
#include "widgets/switches.h"

#include <app/ModuleWidget.hpp>

namespace dhe {
namespace stage {
class UpstagePanel : public rack::app::ModuleWidget {
  using Controls = UpstageControls;
  using Knob = Knobs<UpstagePanel>;
  using Jack = Ports<UpstagePanel>;
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

    addParam(Knob::large(module, column2, y, Controls::LevelKnob));

    y += dy;
    addInput(Jack::input(module, column1, y, Controls::LevelCvInput));
    addParam(Switch::thumb<2>(module, column3, y, Controls::LevelRangeSwitch));

    y += dy;
    addParam(Switch::momentary(module, column1, y, Controls::WaitButton));
    addParam(Switch::momentary(module, column3, y, Controls::TriggerButton));

    y = 82.F;
    dy = 15.F;

    addInput(Jack::input(module, column1, y, Controls::WaitInput));

    y += dy;
    addInput(Jack::input(module, column1, y, Controls::TriggerInput));
    addOutput(Jack::output(module, column3, y, Controls::TriggerOutput));

    y += dy;
    addOutput(Jack::output(module, column3, y, Controls::EnvelopeOutput));
  }
};
} // namespace stage

} // namespace dhe
