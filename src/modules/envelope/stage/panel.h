#pragma once

#include "control-ids.h"

#include "controls/knobs.h"
#include "controls/ports.h"
#include "panels/panel-widget.h"

#include "rack.hpp"

namespace dhe {
namespace envelope {
namespace stage {

struct Panel : public PanelWidget<Panel> {
  static auto constexpr hp = 5;
  static auto constexpr panel_file = "stage";
  static auto constexpr svg_dir = "stage";

  explicit Panel(rack::engine::Module *module) : PanelWidget<Panel>{module} {
    auto constexpr width = hp2mm(hp);

    auto constexpr column1 = width / 4.F + 0.333333F;
    auto constexpr column2 = width / 2.F;
    auto constexpr column3 = width - column1;

    auto y = 25.F;
    auto dy = 18.5F;

    Knob::install<Large>(this, ParamId::Level, column2, y);

    y += dy;
    Knob::install<Large>(this, ParamId::Curvature, column2, y);

    y += dy;
    Knob::install<Large>(this, ParamId::Duration, column2, y);

    y = 82.F;
    dy = 15.F;

    InPort::install(this, InputId::Defer, column1, y);
    OutPort::install(this, OutputId::Active, column3, y);

    y += dy;
    InPort::install(this, InputId::Trigger, column1, y);
    OutPort::install(this, OutputId::Eoc, column3, y);

    y += dy;
    InPort::install(this, InputId::Envelope, column1, y);
    OutPort::install(this, OutputId::Envelope, column3, y);
  }
};
} // namespace stage
} // namespace envelope
} // namespace dhe
