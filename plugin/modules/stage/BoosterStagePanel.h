#pragma once

#include "BoosterStageControls.h"

#include <app/ModuleWidget.hpp>

namespace dhe {
namespace stage {

  class BoosterStagePanel : public rack::app::ModuleWidget {
    using Controls = BoosterStageControls;

  public:
    explicit BoosterStagePanel(rack::engine::Module *module) {
      auto constexpr hp = 8;

      setModule(module);
      setPanel(backgroundSvg(moduleSlug));
      installScrews(this, hp);

      auto width = hp2mm(hp);

      auto column1 = width / 6.F + 0.3333333F;
      auto column3 = width / 2.F;
      auto column5 = width - column1;
      auto buttonPortDistance = 7.891F;
      auto column2 = column1 + buttonPortDistance;
      auto column4 = column5 - buttonPortDistance;

      auto y = 25.F;
      auto dy = 18.5F;

      addInput(Jack::input(moduleSlug, module, column1, y, Controls::LevelCvInput));
      addParam(Knob::large(moduleSlug, module, column3, y, Controls::LevelKnob));

      addParam(Toggle::stepper(2, moduleSlug, module, column5, y, Controls::LevelRangeSwitch));

      y += dy;
      addInput(Jack::input(moduleSlug, module, column1, y, Controls::CurveCvInput));
      addParam(Knob::large(moduleSlug, module, column3, y, Controls::CurveKnob));
      addParam(Toggle::stepper(2, moduleSlug, module, column5, y, Controls::ShapeSwitch));

      y += dy;
      addInput(Jack::input(moduleSlug, module, column1, y, Controls::DurationCvInput));
      addParam(Knob::large(moduleSlug, module, column3, y, Controls::DurationKnob));

      addParam(Toggle::stepper(3, moduleSlug, module, column5, y, Controls::DurationRangeSwitch));

      y = 82.F;
      dy = 15.F;

      addInput(Jack::input(moduleSlug, module, column1, y, Controls::DeferInput));
      addParam(Button::momentary(moduleSlug, module, column2, y, Controls::DeferButton));

      addParam(Button::reverse(moduleSlug, module, column4, y, Controls::ActiveButton));
      addOutput(Jack::output(moduleSlug, module, column5, y, Controls::ActiveOutput));

      y += dy;
      addInput(Jack::input(moduleSlug, module, column1, y, Controls::TriggerInput));
      addParam(Button::momentary(moduleSlug, module, column2, y, Controls::TriggerButton));
      addParam(Button::reverse(moduleSlug, module, column4, y, Controls::EocButton));
      addOutput(Jack::output(moduleSlug, module, column5, y, Controls::EocOutput));

      y += dy;
      addInput(Jack::input(moduleSlug, module, column1, y, Controls::EnvelopeInput));
      addOutput(Jack::output(moduleSlug, module, column5, y, Controls::EnvelopeOutput));
    }
    static constexpr auto moduleSlug = "booster-stage";
  };

} // namespace stage
} // namespace dhe
