#pragma once

#include "upstage-controls.h"

#include <app/ModuleWidget.hpp>

namespace dhe {
namespace stage {
class UpstagePanel : public rack::app::ModuleWidget {
  using Controls = UpstageControls;
  using Button = Button<UpstagePanel>;
  using Jack = Jack<UpstagePanel>;
  using Knob = Knob<UpstagePanel>;
  using Toggle2 = Toggle<UpstagePanel, 2>;

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
    addParam(Toggle2::create(module, column3, y, Controls::LevelRangeSwitch));

    y += dy;
    addParam(Button::momentary(module, column1, y, Controls::WaitButton));
    addParam(Button::momentary(module, column3, y, Controls::TriggerButton));

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
