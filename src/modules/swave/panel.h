#pragma once

#include "control-ids.h"

#include "controls/knobs.h"
#include "controls/ports.h"
#include "controls/switches.h"
#include "panels/panel-widget.h"

#include "rack.hpp"

namespace dhe {
namespace swave {

struct Panel : public PanelWidget<Panel> {
  static auto constexpr hp = 4;
  static auto constexpr panel_file = "swave";
  static auto constexpr svg_dir = "swave";

  explicit Panel(rack::engine::Module *module) : PanelWidget<Panel>{module} {
    auto constexpr width = hp2mm(hp);

    auto constexpr x = width / 2.F;

    auto y = 25.F;
    auto dy = 18.5F;
    ThumbSwitch<2>::install(this, Param::Shape, x, y);

    y += dy;
    Knob::install<Large>(this, Param::Curvature, x, y);

    y += dy;
    Knob::install<Tiny>(this, Param::CurvatureAv, x, y);

    y += dy;
    InPort::install(this, Input::CurvatureCv, x, y);

    y = 82.F;
    dy = 15.F;

    y += dy;
    InPort::install(this, Input::Swave, x, y);

    y += dy;
    OutPort::install(this, Output::Swave, x, y);
  }
};
} // namespace swave
} // namespace dhe
