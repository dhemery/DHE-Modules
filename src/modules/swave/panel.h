#pragma once

#include "control-ids.h"

#include "controls/knobs.h"
#include "controls/ports.h"
#include "controls/shape-controls.h"
#include "controls/switches.h"
#include "widgets/panel-widget.h"

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
    ShapeSwitch::install(this, ParamId::Shape, x, y);

    y += dy;
    Knob::install<Large>(this, ParamId::Curvature, x, y);

    y += dy;
    Knob::install<Tiny>(this, ParamId::CurvatureAv, x, y);

    y += dy;
    InPort::install(this, InputId::CurvatureCv, x, y);

    y = 82.F;
    dy = 15.F;

    y += dy;
    InPort::install(this, InputId::Swave, x, y);

    y += dy;
    OutPort::install(this, OutputId::Swave, x, y);
  }
};
} // namespace swave
} // namespace dhe
