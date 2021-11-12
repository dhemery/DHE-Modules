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

    InPort::install(this, InputId::ACoefficientCv, column1, y);
    Knob::install<Small>(this, ParamId::ACoefficient, column2, y);

    y += dy;
    InPort::install(this, InputId::BCoefficientCv, column1, y);
    Knob::install<Small>(this, ParamId::BCoefficient, column2, y);

    y += dy;
    InPort::install(this, InputId::CCoefficientCv, column1, y);
    Knob::install<Small>(this, ParamId::CCoefficient, column2, y);

    y += dy;
    InPort::install(this, InputId::DCoefficientCv, column1, y);
    Knob::install<Small>(this, ParamId::DCoefficient, column2, y);

    y = 82.F;
    Knob::install<Small>(this, ParamId::InputGain, column1, y);
    Knob::install<Small>(this, ParamId::OutputGain, column2, y);

    y += dy;
    InPort::install(this, InputId::InputGainCv, column1, y);
    InPort::install(this, InputId::OutputGainCv, column2, y);

    y += dy;
    InPort::install(this, InputId::Cubic, column1, y);
    OutPort::install(this, OutputId::Cubic, column2, y);
  }
};
} // namespace cubic
} // namespace dhe
