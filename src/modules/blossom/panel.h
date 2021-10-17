#pragma once

#include "control-ids.h"

#include "controls/ports.h"
#include "controls/steppers.h"
#include "controls/switches.h"
#include "widgets/knobs.h"
#include "widgets/screws.h"

#include "rack.hpp"

namespace dhe {

namespace blossom {

struct Panel : public rack::app::ModuleWidget {
  static auto constexpr svg_dir = "blossom";

  explicit Panel(rack::engine::Module *module) {
    auto constexpr hp = 11;

    setModule(module);
    setPanel(load_svg(svg_dir, "blossom"));
    install_screws(this, hp);

    auto constexpr width = hp2mm(hp);

    auto constexpr column1 = width / 7.F;
    auto constexpr column4 = width - column1;
    auto constexpr column2 = (column4 - column1) / 3.F + column1;
    auto constexpr column3 = width - column2;

    auto y = 25.F;
    auto dy = 18.5F;

    InPort::install(this, Inputs::SpinSpeedCv, column1, y);
    Knob::install<Tiny>(this, Params::SpinSpeedAv, column2, y);
    Knob::install<Large>(this, Params::SpinSpeed, column3, y);

    y += dy;
    InPort::install(this, Inputs::BounceRatioCv, column1, y);
    Knob::install<Tiny>(this, Params::BounceRatioAv, column2, y);
    Knob::install<Large>(this, Params::BounceRatio, column3, y);

    auto quantize_bounce_ratio = [this](int pos) {
      auto *q = reinterpret_cast<BounceRatio::Quantity *>(
          getModule()->getParamQuantity(Params::BounceRatio));
      q->quantize(pos == 0);
    };

    ThumbSwitch<2>::install(this, Params::BounceRatioMode, column4, y,
                            quantize_bounce_ratio);

    y += dy;
    InPort::install(this, Inputs::DepthCv, column1, y);
    Knob::install<Tiny>(this, Params::DepthAv, column2, y);
    Knob::install<Large>(this, Params::Depth, column3, y);

    y += dy;
    InPort::install(this, Inputs::PhaseCv, column1, y);
    Knob::install<Tiny>(this, Params::PhaseAv, column2, y);
    Knob::install<Large>(this, Params::Phase, column3, y);

    y = 97.F;
    dy = 15.F;
    auto constexpr output_port_offset = 1.25F;

    InPort::install(this, Inputs::XGainCv, column1, y);
    Knob::install<Small>(this, Params::XGain, column2, y);
    ThumbSwitch<2>::install(this, Params::XRange, column3, y);
    OutPort::install(this, Outputs::XOutput, column4, y + output_port_offset);

    y += dy;
    OutPort::install(this, Inputs::YGainCv, column1, y);
    Knob::install<Small>(this, Params::YGain, column2, y);
    ThumbSwitch<2>::install(this, Params::YRange, column3, y);
    OutPort::install(this, Outputs::YOutput, column4, y + output_port_offset);
  }
};
} // namespace blossom
} // namespace dhe
