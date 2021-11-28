#pragma once

#include "control-ids.h"

#include "controls/knobs.h"
#include "controls/ports.h"
#include "controls/switches.h"
#include "controls/voltage-controls.h"
#include "panels/panel-widget.h"

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
    Knob::install<Medium>(this, ParamId::Level, left, y);
    OutPort::install(this, OutputId::Main, right, y);

    y += dy;
    InPort::install(this, InputId::LevelCv, left, y);
    Knob::install<Tiny>(this, ParamId::LevelAv, right, y);

    y += dy / 2.F;
    y += dy * 0.75F;
    Knob::install<Medium>(this, ParamId::CcwLimit, left, y);
    Knob::install<Medium>(this, ParamId::CwLimit, right, y);

    y += dy;
    Knob::install<Tiny>(this, ParamId::CcwLimitAv, left, y);
    Knob::install<Tiny>(this, ParamId::CwLimitAv, right, y);

    y += dy;
    InPort::install(this, InputId::CcwLimitCv, left, y);
    InPort::install(this, InputId::CwLimitCv, right, y);

    y += dy;
    VoltageRangeSwitch::install(this, ParamId::CcwLimitRange, left, y);
    VoltageRangeSwitch::install(this, ParamId::CwLimitRange, right, y);
  }
};
} // namespace ranger
} // namespace dhe
