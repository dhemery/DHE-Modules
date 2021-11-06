#pragma once

#include "control-ids.h"

#include "components/range.h"
#include "controls/knobs.h"
#include "controls/ports.h"
#include "controls/switches.h"
#include "signals/levels.h"
#include "widgets/panel-widget.h"

#include "rack.hpp"

namespace dhe {
namespace envelope {
namespace booster_stage {

struct Panel : public PanelWidget<Panel> {
  static auto constexpr hp = 8;
  static auto constexpr panel_file = "booster-stage";
  static auto constexpr svg_dir = "booster-stage";

  explicit Panel(rack::engine::Module *module) : PanelWidget<Panel>{module} {
    auto constexpr width = hp2mm(hp);

    auto constexpr column1 = width / 6.F + 0.3333333F;
    auto constexpr column3 = width / 2.F;
    auto constexpr column5 = width - column1;
    auto constexpr button_port_distance = 7.891F;
    auto constexpr column2 = column1 + button_port_distance;
    auto constexpr column4 = column5 - button_port_distance;

    auto dy = 18.5F;
    auto y = 25.F;

    InPort::install(this, Input::LevelCv, column1, y);
    Knob::install<Large>(this, Param::Level, column3, y);
    ThumbSwitch<2>::install(this, Param::LevelRange, column5, y);

    y += dy;
    InPort::install(this, Input::CurvatureCv, column1, y);
    Knob::install<Large>(this, Param::Curvature, column3, y);
    ThumbSwitch<2>::install(this, Param::Shape, column5, y);

    y += dy;
    InPort::install(this, Input::DurationCv, column1, y);
    Knob::install<Large>(this, Param::Duration, column3, y);

    ThumbSwitch<3>::install(this, Param::DurationRange, column5, y);

    y = 82.F;
    dy = 15.F;

    InPort::install(this, Input::Defer, column1, y);
    Button::install<Momentary>(this, Param::Defer, column2, y);

    Button::install<Momentary, Reversed>(this, Param::Active, column4, y);
    OutPort::install(this, Output::Active, column5, y);

    y += dy;
    InPort::install(this, Input::Trigger, column1, y);
    Button::install<Momentary>(this, Param::Trigger, column2, y);
    Button::install<Momentary, Reversed>(this, Param::Eoc, column4, y);
    OutPort::install(this, Output::Eoc, column5, y);

    y += dy;
    InPort::install(this, Input::Envelope, column1, y);
    OutPort::install(this, Output::Envelope, column5, y);
  }
};

} // namespace booster_stage
} // namespace envelope
} // namespace dhe
