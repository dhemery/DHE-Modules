#pragma once

#include "control-ids.h"
#include "controls.h"
#include "mode-ids.h"

#include "controls/duration-controls.h"
#include "controls/knobs.h"
#include "controls/ports.h"
#include "controls/switches.h"
#include "widgets/panel-widget.h"

#include "rack.hpp"

namespace dhe {
namespace envelope {
namespace hostage {
struct Panel : public PanelWidget<Panel> {

public:
  static auto constexpr hp = 5;
  static auto constexpr panel_file = "hostage";
  static auto constexpr svg_dir = "hostage";

  Panel(rack::engine::Module *module) : PanelWidget<Panel>{module} {
    auto constexpr width = hp2mm(hp);

    auto constexpr column1 = width / 4.F + 0.333333F;
    auto constexpr column2 = width / 2.F;
    auto constexpr column3 = width - column1;

    auto y = 25.F;
    auto dy = 18.5F;

    ModeSwitch::install(this, ParamId::Mode, column2, y);

    y += dy;
    InPort::install(this, InputId::DurationCv, column1, y);
    DurationRangeSwitch::install(this, ParamId::DurationRange, column3, y);

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
} // namespace hostage
} // namespace envelope
} // namespace dhe
