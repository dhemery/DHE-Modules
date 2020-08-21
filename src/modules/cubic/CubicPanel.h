#pragma once

#include "CubicControls.h"
#include "widgets/ControlWidgets.h"
#include "widgets/Screws.h"

#include <app/ModuleWidget.hpp>

namespace dhe {

namespace cubic {

class CubicPanel : public rack::app::ModuleWidget {
  using Controls = CubicControls;

public:
  CubicPanel(rack::engine::Module *module) {
    auto constexpr slug = "cubic";
    auto constexpr hp = 5;

    setModule(module);
    setPanel(background_svg(slug));
    install_screws(this, hp);

    auto constexpr width = hp2mm(hp);

    auto constexpr column1 = width / 4.F + 0.333333F;
    auto constexpr column2 = width - column1;

    auto y = 20.F;
    auto constexpr dy = 15.F;

    addInput(
        Jack::input(slug, module, column1, y, Controls::ACoefficientCvInput));
    addParam(Knob::small(slug, module, column2, y, Controls::ACoefficientKnob));

    y += dy;
    addInput(
        Jack::input(slug, module, column1, y, Controls::BCoefficientCvInput));
    addParam(Knob::small(slug, module, column2, y, Controls::BCoefficientKnob));

    y += dy;
    addInput(
        Jack::input(slug, module, column1, y, Controls::CCoefficientCvInput));
    addParam(Knob::small(slug, module, column2, y, Controls::CCoefficientKnob));

    y += dy;
    addInput(
        Jack::input(slug, module, column1, y, Controls::DCoefficientCvInput));
    addParam(Knob::small(slug, module, column2, y, Controls::DCoefficientKnob));

    y = 82.F;
    addParam(Knob::small(slug, module, column1, y, Controls::InputGainKnob));
    addParam(Knob::small(slug, module, column2, y, Controls::OutputGainKnob));

    y += dy;
    addInput(Jack::input(slug, module, column1, y, Controls::InputGainCvInput));
    addInput(
        Jack::input(slug, module, column2, y, Controls::OutputGainCvInput));

    y += dy;
    addInput(Jack::input(slug, module, column1, y, Controls::CubicInput));
    addOutput(Jack::output(slug, module, column2, y, Controls::CubicOutput));
  }
};
} // namespace cubic
} // namespace dhe
