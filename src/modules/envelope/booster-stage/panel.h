#pragma once

#include "control-ids.h"

#include "components/range.h"
#include "controls/knobs.h"
#include "controls/ports.h"
#include "controls/switches.h"
#include "signals/voltages.h"
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

    InPort::install(this, InputId::LevelCv, column1, y);
    Knob::install<Large>(this, ParamId::Level, column3, y);
    ThumbSwitch::install<VoltageRanges>(this, ParamId::LevelRange, column5, y);

    y += dy;
    InPort::install(this, InputId::CurvatureCv, column1, y);
    Knob::install<Large>(this, ParamId::Curvature, column3, y);
    ThumbSwitch::install<Shapes>(this, ParamId::Shape, column5, y);

    y += dy;
    InPort::install(this, InputId::DurationCv, column1, y);
    Knob::install<Large>(this, ParamId::Duration, column3, y);

    ThumbSwitch::install<DurationRanges>(this, ParamId::DurationRange, column5,
                                         y);

    y = 82.F;
    dy = 15.F;

    InPort::install(this, InputId::Defer, column1, y);
    Button::install<Momentary>(this, ParamId::Defer, column2, y);

    Button::install<Momentary, Reversed>(this, ParamId::Active, column4, y);
    OutPort::install(this, OutputId::Active, column5, y);

    y += dy;
    InPort::install(this, InputId::Trigger, column1, y);
    Button::install<Momentary>(this, ParamId::Trigger, column2, y);
    Button::install<Momentary, Reversed>(this, ParamId::Eoc, column4, y);
    OutPort::install(this, OutputId::Eoc, column5, y);

    y += dy;
    InPort::install(this, InputId::Envelope, column1, y);
    OutPort::install(this, OutputId::Envelope, column5, y);
  }
};

} // namespace booster_stage
} // namespace envelope
} // namespace dhe
