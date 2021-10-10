#pragma once

#include "./xycloid-controls.h"
#include "controls/ports.h"
#include "controls/switches.h"
#include "widgets/knobs.h"
#include "widgets/screws.h"

#include <app/ModuleWidget.hpp>

namespace dhe {

namespace xycloid {

class XycloidPanel : public rack::app::ModuleWidget {
  using Controls = XycloidControls;

public:
  static auto constexpr svg_dir = "xycloid";

  explicit XycloidPanel(rack::engine::Module *module) {
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

    InPort::install(this, Controls::SpeedCvInput, column1, y);
    Knob::install<Tiny>(this, Controls::SpeedAvKnob, column2, y);
    Knob::install<Large>(this, Controls::SpeedKnob, column3, y);

    y += dy;
    InPort::install(this, Controls::RatioCvInput, column1, y);
    Knob::install<Tiny>(this, Controls::RatioAvKnob, column2, y);
    Knob::install<Large>(this, Controls::RatioKnob, column3, y);
    ThumbSwitch<2>::install(this, Controls::FreeRatioSwitch, column4, y);

    y += dy;
    InPort::install(this, Controls::DepthCvInput, column1, y);
    Knob::install<Tiny>(this, Controls::DepthAvKnob, column2, y);
    Knob::install<Large>(this, Controls::DepthKnob, column3, y);
    ThumbSwitch<3>::install(this, Controls::DirectionSwitch, column4, y);

    y += dy;
    InPort::install(this, Controls::PhaseCvInput, column1, y);
    Knob::install<Tiny>(this, Controls::PhaseOffsetAvKnob, column2, y);
    Knob::install<Large>(this, Controls::PhaseOffsetKnob, column3, y);

    y = 82.F;
    dy = 15.F;
    auto constexpr output_port_offset = 1.25F;

    y += dy;
    InPort::install(this, Controls::XGainCvInput, column1, y);
    Knob::install<Small>(this, Controls::XGainKnob, column2, y);
    ThumbSwitch<2>::install(this, Controls::XRangeSwitch, column3, y);
    ThumbSwitch<2>::install(this, Controls::XRangeSwitch, column3, y);
    OutPort::install(this, Controls::XOutput, column4, y + output_port_offset);

    y += dy;
    InPort::install(this, Controls::YGainCvInput, column1, y);
    Knob::install<Small>(this, Controls::YGainKnob, column2, y);
    ThumbSwitch<2>::install(this, Controls::YRangeSwitch, column3, y);
    OutPort::install(this, Controls::YOutput, column4, y + output_port_offset);
  }
};
} // namespace xycloid
} // namespace dhe
