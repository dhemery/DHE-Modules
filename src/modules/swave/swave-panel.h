#pragma once

#include "swave-controls.h"

#include "controls/knobs.h"
#include "controls/ports.h"
#include "controls/switches.h"
#include "widgets/screws.h"

#include "rack.hpp"

namespace dhe {
namespace swave {

class SwavePanel : public rack::app::ModuleWidget {
  using Controls = SwaveControls;

public:
  static auto constexpr svg_dir = "swave";

  explicit SwavePanel(rack::engine::Module *module) {
    auto constexpr hp = 4;

    setModule(module);
    setPanel(load_svg(svg_dir, "swave"));
    install_screws(this, hp);

    auto constexpr width = hp2mm(hp);

    auto constexpr x = width / 2.F;

    auto y = 25.F;
    auto dy = 18.5F;
    ThumbSwitch<2>::install(this, Controls::ShapeSwitch, x, y);

    y += dy;
    Knob::install<Large>(this, Controls::CurveKnob, x, y);

    y += dy;
    Knob::install<Tiny>(this, Controls::CurveAvKnob, x, y);

    y += dy;
    InPort::install(this, Controls::CurveCvInput, x, y);

    y = 82.F;
    dy = 15.F;

    y += dy;
    InPort::install(this, Controls::SwaveInput, x, y);

    y += dy;
    OutPort::install(this, Controls::SwaveOutput, x, y);
  }
};
} // namespace swave
} // namespace dhe
