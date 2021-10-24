#pragma once

#include "control-ids.h"

#include "controls/knobs.h"
#include "controls/ports.h"
#include "controls/switches.h"
#include "panels/panel-widget.h"

#include "rack.hpp"

namespace dhe {
namespace envelope {
namespace upstage {
struct Panel : public PanelWidget<Panel> {
  static auto constexpr hp = 5;
  static auto constexpr panel_file = "upstage";
  static auto constexpr svg_dir = "upstage";

  explicit Panel(rack::engine::Module *module) : PanelWidget<Panel>{module} {
    auto constexpr width = hp2mm(hp);

    auto constexpr column1 = width / 4.F + 0.333333333F;
    auto constexpr column2 = width / 2.F;
    auto constexpr column3 = width - column1;

    auto y = 25.F;
    auto dy = 18.5F;

    Knob::install<Large>(this, Param::LevelKnob, column2, y);

    y += dy;
    InPort::install(this, Input::LevelCvInput, column1, y);
    ThumbSwitch<2>::install(this, Param::LevelRangeSwitch, column3, y);

    y += dy;
    Button::install<Momentary>(this, Param::WaitButton, column1, y);
    Button::install<Momentary>(this, Param::TriggerButton, column3, y);

    y = 82.F;
    dy = 15.F;

    InPort::install(this, Input::WaitInput, column1, y);

    y += dy;
    InPort::install(this, Input::TriggerInput, column1, y);
    OutPort::install(this, Output::TriggerOutput, column3, y);

    y += dy;
    OutPort::install(this, Output::EnvelopeOutput, column3, y);
  }
};
} // namespace upstage
} // namespace envelope

} // namespace dhe
