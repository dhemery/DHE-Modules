#pragma once

#include "./cubic-controls.h"
#include "controls/ports.h"
#include "widgets/knobs.h"
#include "widgets/screws.h"

#include <rack.hpp>

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

    InPort::install(this, Controls::ACoefficientCvInput, column1, y);
    Knob::install<Small>(this, Controls::ACoefficientKnob, column2, y);

    y += dy;
    InPort::install(this, Controls::BCoefficientCvInput, column1, y);
    Knob::install<Small>(this, Controls::BCoefficientKnob, column2, y);

    y += dy;
    InPort::install(this, Controls::CCoefficientCvInput, column1, y);
    Knob::install<Small>(this, Controls::CCoefficientKnob, column2, y);

    y += dy;
    InPort::install(this, Controls::DCoefficientCvInput, column1, y);
    Knob::install<Small>(this, Controls::DCoefficientKnob, column2, y);

    y = 82.F;
    Knob::install<Small>(this, Controls::InputGainKnob, column1, y);
    Knob::install<Small>(this, Controls::OutputGainKnob, column2, y);

    y += dy;
    InPort::install(this, Controls::InputGainCvInput, column1, y);
    InPort::install(this, Controls::OutputGainCvInput, column2, y);

    y += dy;
    InPort::install(this, Controls::CubicInput, column1, y);
    OutPort::install(this, Controls::CubicOutput, column2, y);
  }
};
} // namespace cubic
} // namespace dhe
