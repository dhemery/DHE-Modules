#pragma once

#include "cubic-controls.h"
#include "widgets/control-widgets.h"
#include "widgets/screws.h"

#include <app/ModuleWidget.hpp>

namespace dhe {

namespace cubic {

class CubicPanel : public rack::app::ModuleWidget {
  using Controls = CubicControls;

public:
  static auto constexpr svg_dir = "cubic";

  CubicPanel(rack::engine::Module *module) {
    auto constexpr hp = 5;

    setModule(module);
    setPanel(load_svg(svg_dir, "cubic"));
    install_screws(this, hp);

    auto constexpr width = hp2mm(hp);

    auto constexpr column1 = width / 4.F + 0.333333F;
    auto constexpr column2 = width - column1;

    auto y = 20.F;
    auto constexpr dy = 15.F;

    addInput(Jack::input<CubicPanel>(module, column1, y,
                                     Controls::ACoefficientCvInput));
    addParam(
        Knob::small(svg_dir, module, column2, y, Controls::ACoefficientKnob));

    y += dy;
    addInput(Jack::input<CubicPanel>(module, column1, y,
                                     Controls::BCoefficientCvInput));
    addParam(
        Knob::small(svg_dir, module, column2, y, Controls::BCoefficientKnob));

    y += dy;
    addInput(Jack::input<CubicPanel>(module, column1, y,
                                     Controls::CCoefficientCvInput));
    addParam(
        Knob::small(svg_dir, module, column2, y, Controls::CCoefficientKnob));

    y += dy;
    addInput(Jack::input<CubicPanel>(module, column1, y,
                                     Controls::DCoefficientCvInput));
    addParam(
        Knob::small(svg_dir, module, column2, y, Controls::DCoefficientKnob));

    y = 82.F;
    addParam(Knob::small(svg_dir, module, column1, y, Controls::InputGainKnob));
    addParam(
        Knob::small(svg_dir, module, column2, y, Controls::OutputGainKnob));

    y += dy;
    addInput(Jack::input<CubicPanel>(module, column1, y,
                                     Controls::InputGainCvInput));
    addInput(Jack::input<CubicPanel>(module, column2, y,
                                     Controls::OutputGainCvInput));

    y += dy;
    addInput(Jack::input<CubicPanel>(module, column1, y, Controls::CubicInput));
    addOutput(
        Jack::output<CubicPanel>(module, column2, y, Controls::CubicOutput));
  }
};
} // namespace cubic
} // namespace dhe
