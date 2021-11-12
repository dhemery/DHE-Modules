#pragma once

#include "control-ids.h"
#include "params.h"

#include "controls/knobs.h"
#include "controls/ports.h"
#include "controls/switches.h"
#include "widgets/panel-widget.h"

#include "rack.hpp"

namespace dhe {

namespace xycloid {

struct Panel : public PanelWidget<Panel> {
  static auto constexpr hp = 11;
  static auto constexpr panel_file = "xycloid";
  static auto constexpr svg_dir = "xycloid";

  explicit Panel(rack::engine::Module *module) : PanelWidget<Panel>{module} {
    auto constexpr width = hp2mm(hp);

    auto constexpr column1 = width / 7.F;
    auto constexpr column4 = width - column1;
    auto constexpr column2 = (column4 - column1) / 3.F + column1;
    auto constexpr column3 = width - column2;

    auto y = 25.F;
    auto dy = 18.5F;

    InPort::install(this, Input::ThrobSpeedCv, column1, y);
    Knob::install<Tiny>(this, Param::ThrobSpeedAv, column2, y);
    Knob::install<Large>(this, Param::ThrobSpeed, column3, y);

    y += dy;
    InPort::install(this, Input::WobbleRatioCv, column1, y);
    Knob::install<Tiny>(this, Param::WobbleRatioAv, column2, y);

    Knob::install<Large>(this, Param::WobbleRatio, column3, y);

    ThumbSwitch::install<WobbleRatioModes>(this, Param::WobbleRatioMode,
                                           column4, y);

    y += dy;
    InPort::install(this, Input::WobbleDepthCv, column1, y);
    Knob::install<Tiny>(this, Param::WobbleDepthAv, column2, y);
    Knob::install<Large>(this, Param::WobbleDepth, column3, y);
    ThumbSwitch::install<WobbleRatioRanges>(this, Param::WobbleRatioRange,
                                            column4, y);

    y += dy;
    InPort::install(this, Input::WobblePhaseOffsetCv, column1, y);
    Knob::install<Tiny>(this, Param::WobblePhaseOffsetAv, column2, y);
    Knob::install<Large>(this, Param::WobblePhaseOffset, column3, y);

    y = 82.F;
    dy = 15.F;
    auto constexpr output_port_offset = 1.25F;

    y += dy;
    InPort::install(this, Input::XGainCv, column1, y);
    Knob::install<Small>(this, Param::XGain, column2, y);
    ThumbSwitch::install<Voltages>(this, Param::XRange, column3, y);
    OutPort::install(this, Output::X, column4, y + output_port_offset);

    y += dy;
    InPort::install(this, Input::YGainCv, column1, y);
    Knob::install<Small>(this, Param::YGain, column2, y);
    ThumbSwitch::install<Voltages>(this, Param::YRange, column3, y);
    OutPort::install(this, Output::Y, column4, y + output_port_offset);
  }
  using Input = InputIds;
  using Output = OutputIds;
  using Param = ParamIds;
};
} // namespace xycloid
} // namespace dhe
