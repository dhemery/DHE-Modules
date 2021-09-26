#pragma once

#include "hostage-controls.h"

#include <app/ModuleWidget.hpp>

namespace dhe {
namespace stage {
class HostagePanel : public rack::app::ModuleWidget {
  using Controls = HostageControls;
  using Jack = Jack<HostagePanel>;
  using Knob = Knob<HostagePanel>;
  using Toggle2 = Toggle<HostagePanel, 2>;
  using Toggle3 = Toggle<HostagePanel, 3>;

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

    addParam(Toggle2::create(module, column2, y, Controls::ModeSwitch));

    y += dy;
    addInput(Jack::input(module, column1, y, Controls::DurationCvInput));
    addParam(
        Toggle3::create(module, column3, y, Controls::DurationRangeSwitch));

    y += dy;
    addParam(Knob::large(module, column2, y, Controls::DurationKnob));

    y = 82.F;
    dy = 15.F;

    addInput(Jack::input(module, column1, y, Controls::DeferInput));
    addOutput(Jack::output(module, column3, y, Controls::ActiveOutput));

    y += dy;
    addInput(Jack::input(module, column1, y, Controls::TriggerInput));
    addOutput(Jack::output(module, column3, y, Controls::EocOutput));

    y += dy;
    addInput(Jack::input(module, column1, y, Controls::EnvelopeInput));
    addOutput(Jack::output(module, column3, y, Controls::EnvelopeOutput));
  }
};
} // namespace stage
} // namespace dhe
