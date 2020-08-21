#pragma once

#include "HostageControls.h"

#include <app/ModuleWidget.hpp>

namespace dhe {
namespace stage {
class HostagePanel : public rack::app::ModuleWidget {
  using Controls = HostageControls;

public:
  HostagePanel(rack::engine::Module *module) {
    auto constexpr slug = "hostage";
    auto constexpr hp = 5;

    setModule(module);
    setPanel(backgroundSvg(slug));
    installScrews(this, hp);

    auto constexpr width = hp2mm(hp);

    auto constexpr column1 = width / 4.F + 0.333333F;
    auto constexpr column2 = width / 2.F;
    auto constexpr column3 = width - column1;

    auto y = 25.F;
    auto dy = 18.5F;

    addParam(
        Toggle::stepper(2, slug, module, column2, y, Controls::ModeSwitch));

    y += dy;
    addInput(Jack::input(slug, module, column1, y, Controls::DurationCvInput));
    addParam(Toggle::stepper(3, slug, module, column3, y,
                             Controls::DurationRangeSwitch));

    y += dy;
    addParam(Knob::large(slug, module, column2, y, Controls::DurationKnob));

    y = 82.F;
    dy = 15.F;

    addInput(Jack::input(slug, module, column1, y, Controls::DeferInput));
    addOutput(Jack::output(slug, module, column3, y, Controls::ActiveOutput));

    y += dy;
    addInput(Jack::input(slug, module, column1, y, Controls::TriggerInput));
    addOutput(Jack::output(slug, module, column3, y, Controls::EocOutput));

    y += dy;
    addInput(Jack::input(slug, module, column1, y, Controls::EnvelopeInput));
    addOutput(Jack::output(slug, module, column3, y, Controls::EnvelopeOutput));
  }
};
} // namespace stage
} // namespace dhe
