#pragma once

#include "swave-controls.h"
#include "widgets/control-widgets.h"
#include "widgets/screws.h"

#include <app/ModuleWidget.hpp>

namespace dhe {
namespace swave {

class SwavePanel : public rack::app::ModuleWidget {
  using Controls = SwaveControls;
  using Jack = Jack<SwavePanel>;
  using Knob = Knob<SwavePanel>;

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

    addParam(Toggle::thumb(2, svg_dir, module, x, y, Controls::ShapeSwitch));

    y += dy;
    addParam(Knob::large(module, x, y, Controls::CurveKnob));

    y += dy;
    addParam(Knob::tiny(module, x, y, Controls::CurveAvKnob));

    y += dy;
    addInput(Jack::input(module, x, y, Controls::CurveCvInput));

    y = 82.F;
    dy = 15.F;

    y += dy;
    addInput(Jack::input(module, x, y, Controls::SwaveInput));

    y += dy;
    addOutput(Jack::output(module, x, y, Controls::SwaveOutput));
  }
};
} // namespace swave
} // namespace dhe
