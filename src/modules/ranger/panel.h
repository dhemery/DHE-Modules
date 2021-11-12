#pragma once

#include "control-ids.h"

#include "controls/knobs.h"
#include "controls/ports.h"
#include "controls/switches.h"
#include "widgets/panel-widget.h"

#include "rack.hpp"

namespace dhe {
namespace ranger {

struct Panel : public PanelWidget<Panel> {
  static auto constexpr hp = 6;
  static auto constexpr panel_file = "ranger";
  static auto constexpr svg_dir = "ranger";

  explicit Panel(rack::engine::Module *module) : PanelWidget<Panel>{module} {
    auto constexpr left = hp2mm(1.5F);
    auto constexpr right = hp2mm(hp) - left;

    auto constexpr dy = 15.F;
    auto y = 14.5F;

    y += dy * 0.75F;
    Knob::install<Medium>(this, Param::Level, left, y);
    OutPort::install(this, Output::Main, right, y);

    y += dy;
    InPort::install(this, Input::LevelCv, left, y);
    Knob::install<Tiny>(this, Param::LevelAv, right, y);

    y += dy / 2.F;
    y += dy * 0.75F;
    Knob::install<Medium>(this, Param::CcwLimit, left, y);
    Knob::install<Medium>(this, Param::CwLimit, right, y);

    y += dy;
    Knob::install<Tiny>(this, Param::CcwLimitAv, left, y);
    Knob::install<Tiny>(this, Param::CwLimitAv, right, y);

    y += dy;
    InPort::install(this, Input::CcwLimitCv, left, y);
    InPort::install(this, Input::CwLimitCv, right, y);

    y += dy;
    ThumbSwitch::install<VoltageRanges>(this, Param::CcwLimitRange, left, y);
    ThumbSwitch::install<VoltageRanges>(this, Param::CwLimitRange, right, y);
  }
};
} // namespace ranger
} // namespace dhe
