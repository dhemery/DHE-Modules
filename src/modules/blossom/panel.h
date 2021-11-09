#pragma once

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

    InPort::install(this, Input::SpinSpeedCv, column1, y);
    Knob::install<Tiny>(this, Param::SpinSpeedAv, column2, y);
    Knob::install<Large>(this, Param::SpinSpeed, column3, y);

    y += dy;
    InPort::install(this, Input::BounceRatioCv, column1, y);
    Knob::install<Tiny>(this, Param::BounceRatioAv, column2, y);
    Knob::install<Large>(this, Param::BounceRatio, column3, y);

    auto quantize_bounce_ratio = [this](int pos) {
      auto *q = reinterpret_cast<BounceRatio::Quantity *>(
          getModule()->getParamQuantity(Param::BounceRatio));
      q->quantize(pos == 0);
    };

    ThumbSwitch::install<BounceRatioModes>(this, Param::BounceRatioMode,
                                           column4, y)
        ->on_change(quantize_bounce_ratio);

    y += dy;
    InPort::install(this, Input::BounceDepthCv, column1, y);
    Knob::install<Tiny>(this, Param::BounceDepthAv, column2, y);
    Knob::install<Large>(this, Param::BounceDepth, column3, y);

    y += dy;
    InPort::install(this, Input::BouncePhaseOffsetCv, column1, y);
    Knob::install<Tiny>(this, Param::BouncePhaseOffsetAv, column2, y);
    Knob::install<Large>(this, Param::BouncePhaseOffset, column3, y);

    y = 97.F;
    dy = 15.F;
    auto constexpr output_port_offset = 1.25F;

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
};
} // namespace blossom
} // namespace dhe
