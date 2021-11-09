#pragma once

#include "control-ids.h"

#include "controls/knobs.h"
#include "controls/ports.h"
#include "controls/switches.h"
#include "widgets/panel-widget.h"

#include "rack.hpp"

namespace dhe {
namespace tapers {

struct Panel : public PanelWidget<Panel> {
  static auto constexpr hp = 9;
  static auto constexpr panel_file = "tapers";
  static auto constexpr svg_dir = "tapers";

  explicit Panel(rack::engine::Module *module) : PanelWidget<Panel>{module} {
    auto constexpr width = hp2mm(hp);

    auto constexpr column1 = width / 5.F + 0.333333333F;
    auto constexpr column2 = width / 2.F;
    auto constexpr column3 = width - column1;

    auto constexpr dy = 16.F;
    auto constexpr panel_buffer = 4.F;
    auto y = 24.F;

    InPort::install(this, Input::LevelCv1, column1, y);
    Knob::install<Tiny>(this, Param::LevelAv1, column2, y);
    Knob::install<Medium>(this, Param::Level1, column3, y);
    y += dy;
    InPort::install(this, Input::CurvatureCv1, column1, y);
    Knob::install<Tiny>(this, Param::CurvatureAv1, column2, y);
    Knob::install<Medium>(this, Param::Curvature1, column3, y);
    y += dy;
    ThumbSwitch::install<Shapes>(this, Param::Shape1, column1, y);
    ThumbSwitch::install<Voltages>(this, Param::LevelRange1, column2, y);
    OutPort::install(this, Output::Taper1, column3, y);

    y += dy + panel_buffer;

    InPort::install(this, Input::LevelCv2, column1, y);
    Knob::install<Tiny>(this, Param::LevelAv2, column2, y);
    Knob::install<Medium>(this, Param::Level2, column3, y);
    y += dy;
    InPort::install(this, Input::CurvatureCv2, column1, y);
    Knob::install<Tiny>(this, Param::CurvatureAv2, column2, y);
    Knob::install<Medium>(this, Param::Curvature2, column3, y);
    y += dy;
    ThumbSwitch::install<Shapes>(this, Param::Shape2, column1, y);
    ThumbSwitch::install<Voltages>(this, Param::LevelRange2, column2, y);
    OutPort::install(this, Output::Taper2, column3, y);
  }
};
} // namespace tapers
} // namespace dhe
