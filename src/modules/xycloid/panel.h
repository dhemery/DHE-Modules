#pragma once

#include "control-ids.h"
#include "params.h"

#include "controls/knobs.h"
#include "controls/ports.h"
#include "controls/switches.h"
#include "panels/screws.h"

#include "rack.hpp"

namespace dhe {

namespace xycloid {

class Panel : public rack::app::ModuleWidget {
public:
  static auto constexpr svg_dir = "xycloid";

  explicit Panel(rack::engine::Module *module) {
    auto constexpr hp = 11;

    setModule(module);
    setPanel(load_svg(svg_dir, "xycloid"));
    install_screws(this, hp);

    auto constexpr width = hp2mm(hp);

    auto constexpr column1 = width / 7.F;
    auto constexpr column4 = width - column1;
    auto constexpr column2 = (column4 - column1) / 3.F + column1;
    auto constexpr column3 = width - column2;

    auto y = 25.F;
    auto dy = 18.5F;

    InPort::install(this, Input::SpeedCvInput, column1, y);
    Knob::install<Tiny>(this, Param::SpeedAvKnob, column2, y);
    Knob::install<Large>(this, Param::SpeedKnob, column3, y);

    y += dy;
    InPort::install(this, Input::RatioCvInput, column1, y);
    Knob::install<Tiny>(this, Param::RatioAvKnob, column2, y);
    Knob::install<Large>(this, Param::RatioKnob, column3, y);

    auto quantize_wobble_ratio = [this](int pos) {
      auto *q = reinterpret_cast<WobbleRatio::Quantity *>(
          getModule()->getParamQuantity(Param::RatioKnob));
      q->quantize(pos == 0);
    };
    ThumbSwitch<2>::install(this, Param::FreeRatioSwitch, column4, y,
                            quantize_wobble_ratio);

    auto select_wobble_ratio_range = [this](int pos) {
      auto *q = reinterpret_cast<WobbleRatio::Quantity *>(
          getModule()->getParamQuantity(Param::RatioKnob));
      q->select_range(pos);
    };

    y += dy;
    InPort::install(this, Input::DepthCvInput, column1, y);
    Knob::install<Tiny>(this, Param::DepthAvKnob, column2, y);
    Knob::install<Large>(this, Param::DepthKnob, column3, y);
    ThumbSwitch<3>::install(this, Param::DirectionSwitch, column4, y,
                            select_wobble_ratio_range);

    y += dy;
    InPort::install(this, Input::PhaseCvInput, column1, y);
    Knob::install<Tiny>(this, Param::PhaseOffsetAvKnob, column2, y);
    Knob::install<Large>(this, Param::PhaseOffsetKnob, column3, y);

    y = 82.F;
    dy = 15.F;
    auto constexpr output_port_offset = 1.25F;

    y += dy;
    InPort::install(this, Input::XGainCvInput, column1, y);
    Knob::install<Small>(this, Param::XGainKnob, column2, y);
    ThumbSwitch<2>::install(this, Param::XRangeSwitch, column3, y);
    OutPort::install(this, Output::XOutput, column4, y + output_port_offset);

    y += dy;
    InPort::install(this, Input::YGainCvInput, column1, y);
    Knob::install<Small>(this, Param::YGainKnob, column2, y);
    ThumbSwitch<2>::install(this, Param::YRangeSwitch, column3, y);
    OutPort::install(this, Output::YOutput, column4, y + output_port_offset);
  }
  using Input = InputIds;
  using Output = OutputIds;
  using Param = ParamIds;
};
} // namespace xycloid
} // namespace dhe
