#pragma once

#include "control-ids.h"

#include "controls/knobs.h"
#include "controls/ports.h"
#include "controls/shape-controls.h"
#include "controls/switches.h"
#include "controls/voltage-controls.h"
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
    auto constexpr shape_x = width / 4.F;
    auto constexpr level_x = width - shape_x;
    ShapeSwitch::install(this, ParamId::Shape, shape_x, y);
    VoltageRangeSwitch::install(this, ParamId::Level, level_x, y);

    y += dy;
    Knob::install<Large>(this, ParamId::Curvature, x, y);

    y += dy;
    auto constexpr cv_av_padding =
        (width - port_diameter - tiny_knob_diameter) / 3.F;
    auto constexpr cv_x = cv_av_padding + port_radius;
    auto constexpr av_x = width - cv_av_padding - tiny_knob_radius;
    InPort::install(this, InputId::CurvatureCv, cv_x, y);
    Knob::install<Tiny>(this, ParamId::CurvatureAv, av_x, y);

    y = 90.F;
    dy = 15.F;

    InPort::install(this, InputId::Swave, x, y);
    y += dy;
    OutPort::install(this, OutputId::Swave, x, y);
  }
};
} // namespace swave
} // namespace dhe
