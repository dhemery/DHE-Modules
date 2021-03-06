#pragma once

#include "ranger-controls.h"
#include "widgets/control-widgets.h"
#include "widgets/screws.h"

#include <app/ModuleWidget.hpp>

namespace dhe {
namespace ranger {

class RangerPanel : public rack::app::ModuleWidget {
  using Controls = RangerControls;

public:
  explicit RangerPanel(rack::engine::Module *module) {
    auto constexpr svg_dir = "ranger";
    auto constexpr hp = 6;

    setModule(module);
    setPanel(load_svg(svg_dir, "ranger"));
    install_screws(this, hp);

    auto constexpr left = hp2mm(1.5F);
    auto constexpr right = hp2mm(hp) - left;

    auto constexpr dy = 15.F;
    auto y = 14.5F;

    y += dy * 0.75F;
    addParam(Knob::medium(svg_dir, module, left, y, Controls::LevelKnob));
    addOutput(Jack::output(svg_dir, module, right, y, Controls::RangerOutput));

    y += dy;
    addInput(Jack::input(svg_dir, module, left, y, Controls::LevelCvInput));
    addParam(Knob::tiny(svg_dir, module, right, y, Controls::LevelAvKnob));

    y += dy / 2.F;
    y += dy * 0.75F;
    addParam(Knob::medium(svg_dir, module, left, y, Controls::CcwLimitKnob));
    addParam(Knob::medium(svg_dir, module, right, y, Controls::CwLimitKnob));

    y += dy;
    addParam(Knob::tiny(svg_dir, module, left, y, Controls::CcwLimitAvKnob));
    addParam(Knob::tiny(svg_dir, module, right, y, Controls::CwLimitAvKnob));

    y += dy;
    addInput(Jack::input(svg_dir, module, left, y, Controls::CcwLimitCvInput));
    addInput(Jack::input(svg_dir, module, right, y, Controls::CwLimitCvInput));

    y += dy;
    addParam(Toggle::thumb(2, svg_dir, module, left, y,
                           Controls::CcwLimitRangeSwitch));
    addParam(Toggle::thumb(2, svg_dir, module, right, y,
                           Controls::CwLimitRangeSwitch));
  }
};
} // namespace ranger
} // namespace dhe
