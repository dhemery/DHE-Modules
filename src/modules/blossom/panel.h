#pragma once

#include "bounce-ratio.h"
#include "control-ids.h"

#include "controls/knobs.h"
#include "controls/ports.h"
#include "controls/switches.h"
#include "signals/voltages.h"
#include "widgets/panel-widget.h"

#include "rack.hpp"

namespace dhe {

namespace blossom {

struct Panel : public PanelWidget<Panel> {
  static auto constexpr hp = 11;
  static auto constexpr panel_file = "blossom";
  static auto constexpr svg_dir = "blossom";

  explicit Panel(rack::engine::Module *module) : PanelWidget<Panel>{module} {
    auto constexpr width = hp2mm(hp);
    auto constexpr column1 = width / 7.F;
    auto constexpr column4 = width - column1;
    auto constexpr column2 = (column4 - column1) / 3.F + column1;
    auto constexpr column3 = width - column2;

    auto y = 25.F;
    auto dy = 18.5F;

    InPort::install(this, InputId::SpinSpeedCv, column1, y);
    Knob::install<Tiny>(this, ParamId::SpinSpeedAv, column2, y);
    Knob::install<Large>(this, ParamId::SpinSpeed, column3, y);

    y += dy;
    InPort::install(this, InputId::BounceRatioCv, column1, y);
    Knob::install<Tiny>(this, ParamId::BounceRatioAv, column2, y);
    Knob::install<Large>(this, ParamId::BounceRatio, column3, y);

    ThumbSwitch::install<BounceRatioModes>(this, ParamId::BounceRatioMode,
                                           column4, y);

    y += dy;
    InPort::install(this, InputId::BounceDepthCv, column1, y);
    Knob::install<Tiny>(this, ParamId::BounceDepthAv, column2, y);
    Knob::install<Large>(this, ParamId::BounceDepth, column3, y);

    y += dy;
    InPort::install(this, InputId::BouncePhaseOffsetCv, column1, y);
    Knob::install<Tiny>(this, ParamId::BouncePhaseOffsetAv, column2, y);
    Knob::install<Large>(this, ParamId::BouncePhaseOffset, column3, y);

    y = 97.F;
    dy = 15.F;
    auto constexpr output_port_offset = 1.25F;

    InPort::install(this, InputId::XGainCv, column1, y);
    Knob::install<Small>(this, ParamId::XGain, column2, y);
    ThumbSwitch::install<VoltageRanges>(this, ParamId::XRange, column3, y);
    OutPort::install(this, OutputId::X, column4, y + output_port_offset);

    y += dy;
    InPort::install(this, InputId::YGainCv, column1, y);
    Knob::install<Small>(this, ParamId::YGain, column2, y);
    ThumbSwitch::install<VoltageRanges>(this, ParamId::YRange, column3, y);
    OutPort::install(this, OutputId::Y, column4, y + output_port_offset);
  }
};
} // namespace blossom
} // namespace dhe
