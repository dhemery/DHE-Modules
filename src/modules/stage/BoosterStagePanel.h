#pragma once

#include "BoosterStageControls.h"

#include <app/ModuleWidget.hpp>

namespace dhe {
namespace stage {

class BoosterStagePanel : public rack::app::ModuleWidget {
  using Controls = BoosterStageControls;

public:
  explicit BoosterStagePanel(rack::engine::Module *module) {
    auto constexpr slug = "booster-stage";
    auto constexpr hp = 8;

    setModule(module);
    setPanel(background_svg(slug));
    install_screws(this, hp);

    auto constexpr width = hp2mm(hp);

    auto constexpr column1 = width / 6.F + 0.3333333F;
    auto constexpr column3 = width / 2.F;
    auto constexpr column5 = width - column1;
    auto constexpr buttonPortDistance = 7.891F;
    auto constexpr column2 = column1 + buttonPortDistance;
    auto constexpr column4 = column5 - buttonPortDistance;

    auto dy = 18.5F;
    auto y = 25.F;

    addInput(Jack::input(slug, module, column1, y, Controls::LevelCvInput));
    addParam(Knob::large(slug, module, column3, y, Controls::LevelKnob));

    addParam(Toggle::stepper(2, slug, module, column5, y,
                             Controls::LevelRangeSwitch));

    y += dy;
    addInput(Jack::input(slug, module, column1, y, Controls::CurveCvInput));
    addParam(Knob::large(slug, module, column3, y, Controls::CurveKnob));
    addParam(
        Toggle::stepper(2, slug, module, column5, y, Controls::ShapeSwitch));

    y += dy;
    addInput(Jack::input(slug, module, column1, y, Controls::DurationCvInput));
    addParam(Knob::large(slug, module, column3, y, Controls::DurationKnob));

    addParam(Toggle::stepper(3, slug, module, column5, y,
                             Controls::DurationRangeSwitch));

    y = 82.F;
    dy = 15.F;

    addInput(Jack::input(slug, module, column1, y, Controls::DeferInput));
    addParam(
        Button::momentary(slug, module, column2, y, Controls::DeferButton));

    addParam(Button::reverse(slug, module, column4, y, Controls::ActiveButton));
    addOutput(Jack::output(slug, module, column5, y, Controls::ActiveOutput));

    y += dy;
    addInput(Jack::input(slug, module, column1, y, Controls::TriggerInput));
    addParam(
        Button::momentary(slug, module, column2, y, Controls::TriggerButton));
    addParam(Button::reverse(slug, module, column4, y, Controls::EocButton));
    addOutput(Jack::output(slug, module, column5, y, Controls::EocOutput));

    y += dy;
    addInput(Jack::input(slug, module, column1, y, Controls::EnvelopeInput));
    addOutput(Jack::output(slug, module, column5, y, Controls::EnvelopeOutput));
  }
};

} // namespace stage
} // namespace dhe
