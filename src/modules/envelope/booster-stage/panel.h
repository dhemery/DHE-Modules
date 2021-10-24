#pragma once

#include "control-ids.h"

#include "controls/knobs.h"
#include "controls/ports.h"
#include "controls/switches.h"
#include "panels/panel-widget.h"

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

    InPort::install(this, Input::LevelCvInput, column1, y);
    Knob::install<Large>(this, Param::LevelKnob, column3, y);

    ThumbSwitch<2>::install(this, Param::LevelRangeSwitch, column5, y);

    y += dy;
    InPort::install(this, Input::CurveCvInput, column1, y);
    Knob::install<Large>(this, Param::CurveKnob, column3, y);
    ThumbSwitch<2>::install(this, Param::ShapeSwitch, column5, y);

    y += dy;
    InPort::install(this, Input::DurationCvInput, column1, y);
    Knob::install<Large>(this, Param::DurationKnob, column3, y);

    ThumbSwitch<3>::install(this, Param::DurationRangeSwitch, column5, y);

    y = 82.F;
    dy = 15.F;

    InPort::install(this, Input::DeferInput, column1, y);
    Button::install<Momentary>(this, Param::DeferButton, column2, y);

    Button::install<Momentary, Reversed>(this, Param::ActiveButton, column4, y);
    OutPort::install(this, Output::ActiveOutput, column5, y);

    y += dy;
    InPort::install(this, Input::TriggerInput, column1, y);
    Button::install<Momentary>(this, Param::TriggerButton, column2, y);
    Button::install<Momentary, Reversed>(this, Param::EocButton, column4, y);
    OutPort::install(this, Output::EocOutput, column5, y);

    y += dy;
    InPort::install(this, Input::EnvelopeInput, column1, y);
    OutPort::install(this, Output::EnvelopeOutput, column5, y);
  }
};

} // namespace booster_stage
} // namespace envelope
} // namespace dhe
