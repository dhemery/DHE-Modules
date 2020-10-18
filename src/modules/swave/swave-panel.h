#pragma once

#include "swave-controls.h"
#include "widgets/control-widgets.h"
#include "widgets/screws.h"

#include <app/ModuleWidget.hpp>

namespace dhe {
namespace swave {

class SwavePanel : public rack::app::ModuleWidget {
  using Controls = SwaveControls;

public:
  explicit SwavePanel(rack::engine::Module *module) {
    auto constexpr slug = "swave";
    auto constexpr hp = 4;

    setModule(module);
    setPanel(background_svg(slug));
    install_screws(this, hp);

    auto constexpr width = hp2mm(hp);

    auto constexpr x = width / 2.F;

    auto y = 25.F;
    auto dy = 18.5F;

    addParam(Toggle::thumb(2, slug, module, x, y, Controls::ShapeSwitch));

    y += dy;
    addParam(Knob::large(slug, module, x, y, Controls::CurveKnob));

    y += dy;
    addParam(Knob::tiny(slug, module, x, y, Controls::CurveAvKnob));

    y += dy;
    addInput(Jack::input(slug, module, x, y, Controls::CurveCvInput));

    y = 82.F;
    dy = 15.F;

    y += dy;
    addInput(Jack::input(slug, module, x, y, Controls::SwaveInput));

    y += dy;
    addOutput(Jack::output(slug, module, x, y, Controls::SwaveOutput));
  }
};
} // namespace swave
} // namespace dhe