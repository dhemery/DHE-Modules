#pragma once

#include "RangerControls.h"
#include "widgets/ControlWidgets.h"
#include "widgets/Screws.h"

#include <app/ModuleWidget.hpp>

namespace dhe {
namespace ranger {

class RangerPanel : public rack::app::ModuleWidget {
  using Controls = RangerControls;

public:
  explicit RangerPanel(rack::engine::Module *module) {
    auto constexpr slug = "ranger";
    auto constexpr hp = 6;

    setModule(module);
    setPanel(backgroundSvg(slug));
    installScrews(this, hp);

    auto constexpr left = hp2mm(1.5F);
    auto constexpr right = hp2mm(hp) - left;

    auto constexpr dy = 15.F;
    auto y = 14.5F;

    y += dy * 0.75F;
    addParam(Knob::medium(slug, module, left, y, Controls::LevelKnob));
    addOutput(Jack::output(slug, module, right, y, Controls::RangerOutput));

    y += dy;
    addInput(Jack::input(slug, module, left, y, Controls::LevelCvInput));
    addParam(Knob::tiny(slug, module, right, y, Controls::LevelAvKnob));

    y += dy / 2.F;
    y += dy * 0.75F;
    addParam(Knob::medium(slug, module, left, y, Controls::CcwLimitKnob));
    addParam(Knob::medium(slug, module, right, y, Controls::CwLimitKnob));

    y += dy;
    addParam(Knob::tiny(slug, module, left, y, Controls::CcwLimitAvKnob));
    addParam(Knob::tiny(slug, module, right, y, Controls::CwLimitAvKnob));

    y += dy;
    addInput(Jack::input(slug, module, left, y, Controls::CcwLimitCvInput));
    addInput(Jack::input(slug, module, right, y, Controls::CwLimitCvInput));

    y += dy;
    addParam(Toggle::stepper(2, slug, module, left, y,
                             Controls::CcwLimitRangeSwitch));
    addParam(Toggle::stepper(2, slug, module, right, y,
                             Controls::CwLimitRangeSwitch));
  }
};
} // namespace ranger
} // namespace dhe
