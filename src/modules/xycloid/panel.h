#pragma once

#include "control-ids.h"
#include "controls.h"
#include "throb-speed.h"
#include "wobble-ratio.h"

#include "controls/knobs.h"
#include "controls/ports.h"
#include "controls/switches.h"
#include "controls/voltage-controls.h"
#include "panels/panel-widget.h"

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

    InPort::install(this, InputId::ThrobSpeedCv, column1, y);
    Knob::install<Tiny>(this, ParamId::ThrobSpeedAv, column2, y);
    Knob::install<Large>(this, ParamId::ThrobSpeed, column3, y);

    y += dy;
    InPort::install(this, InputId::WobbleRatioCv, column1, y);
    Knob::install<Tiny>(this, ParamId::WobbleRatioAv, column2, y);

    Knob::install<Large>(this, ParamId::WobbleRatio, column3, y);

    WobbleRatioModeSwitch::install(this, ParamId::WobbleRatioMode, column4, y);

    y += dy;
    InPort::install(this, InputId::WobbleDepthCv, column1, y);
    Knob::install<Tiny>(this, ParamId::WobbleDepthAv, column2, y);
    Knob::install<Large>(this, ParamId::WobbleDepth, column3, y);
    WobbleRatioRangeSwitch::install(this, ParamId::WobbleRatioRange, column4,
                                    y);

    y += dy;
    InPort::install(this, InputId::WobblePhaseOffsetCv, column1, y);
    Knob::install<Tiny>(this, ParamId::WobblePhaseOffsetAv, column2, y);
    Knob::install<Large>(this, ParamId::WobblePhaseOffset, column3, y);

    y = 82.F;
    dy = 15.F;
    auto constexpr output_port_offset = 1.25F;

    y += dy;
    InPort::install(this, InputId::XGainCv, column1, y);
    Knob::install<Small>(this, ParamId::XGain, column2, y);
    VoltageRangeSwitch::install(this, ParamId::XRange, column3, y);
    OutPort::install(this, OutputId::X, column4, y + output_port_offset);

    y += dy;
    InPort::install(this, InputId::YGainCv, column1, y);
    Knob::install<Small>(this, ParamId::YGain, column2, y);
    VoltageRangeSwitch::install(this, ParamId::YRange, column3, y);
    OutPort::install(this, OutputId::Y, column4, y + output_port_offset);
  }
};
} // namespace xycloid
} // namespace dhe
