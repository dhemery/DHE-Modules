#pragma once

#include "control-ids.h"

#include "controls/knobs.h"
#include "controls/ports.h"
#include "widgets/panel-widget.h"

#include "rack.hpp"

namespace dhe {

namespace cubic {

struct Panel : public PanelWidget<Panel> {
  static auto constexpr hp = 5;
  static auto constexpr panel_file = "cubic";
  static auto constexpr svg_dir = "cubic";

  Panel(rack::engine::Module *module) : PanelWidget<Panel>{module} {
    auto constexpr width = hp2mm(hp);

    auto constexpr column1 = width / 4.F + 0.333333F;
    auto constexpr column2 = width - column1;

    auto y = 20.F;
    auto constexpr dy = 15.F;

    InPort::install(this, Input::ACoefficientCv, column1, y);
    Knob::install<Small>(this, Param::ACoefficient, column2, y);

    y += dy;
    InPort::install(this, Input::BCoefficientCv, column1, y);
    Knob::install<Small>(this, Param::BCoefficient, column2, y);

    y += dy;
    InPort::install(this, Input::CCoefficientCv, column1, y);
    Knob::install<Small>(this, Param::CCoefficient, column2, y);

    y += dy;
    InPort::install(this, Input::DCoefficientCv, column1, y);
    Knob::install<Small>(this, Param::DCoefficient, column2, y);

    y = 82.F;
    Knob::install<Small>(this, Param::InputGain, column1, y);
    Knob::install<Small>(this, Param::OutputGain, column2, y);

    y += dy;
    InPort::install(this, Input::InputGainCv, column1, y);
    InPort::install(this, Input::OutputGainCv, column2, y);

    y += dy;
    InPort::install(this, Input::Cubic, column1, y);
    OutPort::install(this, Output::Cubic, column2, y);
  }
};
} // namespace cubic
} // namespace dhe
