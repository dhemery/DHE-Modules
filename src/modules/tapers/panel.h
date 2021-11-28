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
namespace tapers {

struct Panel : public PanelWidget<Panel> {
  static auto constexpr hp = 9;
  static auto constexpr panel_file = "tapers";
  static auto constexpr svg_dir = "tapers";

  explicit Panel(rack::engine::Module *module) : PanelWidget<Panel>{module} {
    auto constexpr width = hp2mm(hp);

    auto constexpr column1 = width / 5.F + 0.333333333F;
    auto constexpr column2 = width / 2.F;
    auto constexpr column3 = width - column1;

    auto constexpr dy = 16.F;
    auto constexpr panel_buffer = 4.F;
    auto y = 24.F;

    InPort::install(this, InputId::LevelCv1, column1, y);
    Knob::install<Tiny>(this, ParamId::LevelAv1, column2, y);
    Knob::install<Medium>(this, ParamId::Level1, column3, y);
    y += dy;
    InPort::install(this, InputId::CurvatureCv1, column1, y);
    Knob::install<Tiny>(this, ParamId::CurvatureAv1, column2, y);
    Knob::install<Medium>(this, ParamId::Curvature1, column3, y);
    y += dy;
    ShapeSwitch::install(this, ParamId::Shape1, column1, y);
    VoltageRangeSwitch::install(this, ParamId::LevelRange1, column2, y);
    OutPort::install(this, OutputId::Taper1, column3, y);

    y += dy + panel_buffer;

    InPort::install(this, InputId::LevelCv2, column1, y);
    Knob::install<Tiny>(this, ParamId::LevelAv2, column2, y);
    Knob::install<Medium>(this, ParamId::Level2, column3, y);
    y += dy;
    InPort::install(this, InputId::CurvatureCv2, column1, y);
    Knob::install<Tiny>(this, ParamId::CurvatureAv2, column2, y);
    Knob::install<Medium>(this, ParamId::Curvature2, column3, y);
    y += dy;
    ShapeSwitch::install(this, ParamId::Shape2, column1, y);
    VoltageRangeSwitch::install(this, ParamId::LevelRange2, column2, y);
    OutPort::install(this, OutputId::Taper2, column3, y);
  }
};
} // namespace tapers
} // namespace dhe
