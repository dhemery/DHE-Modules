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

      installInput(this, module, column1, y, Controls::LevelCvInput);
      install<LargeKnob>(this, module, column3, y, Controls::LevelKnob);

      install<Toggle<2>>(this, module, column5, y, Controls::LevelRangeSwitch);

      y += dy;
      installInput(this, module, column1, y, Controls::CurveCvInput);
      install<LargeKnob>(this, module, column3, y, Controls::CurveKnob);
      install<Toggle<2>>(this, module, column5, y, Controls::ShapeSwitch);

      y += dy;
      installInput(this, module, column1, y, Controls::DurationCvInput);
      install<LargeKnob>(this, module, column3, y, Controls::DurationKnob);

      install<Toggle<3>>(this, module, column5, y, Controls::DurationRangeSwitch);

      y = 82.F;
      dy = 15.F;

      installInput(this, module, column1, y, Controls::DeferInput);
      install<MomentaryButton>(this, module, column2, y, Controls::DeferButton);

      install<ReverseMomentaryButton>(this, module, column4, y, Controls::ActiveButton);
      installOutput(this, module, column5, y, Controls::ActiveOutput);

      y += dy;
      installInput(this, module, column1, y, Controls::TriggerInput);
      install<MomentaryButton>(this, module, column2, y, Controls::TriggerButton);
      install<ReverseMomentaryButton>(this, module, column4, y, Controls::EocButton);
      installOutput(this, module, column5, y, Controls::EocOutput);

      y += dy;
      installInput(this, module, column1, y, Controls::EnvelopeInput);
      installOutput(this, module, column5, y, Controls::EnvelopeOutput);
    }
    static constexpr auto moduleSlug = "booster-stage";
  };

} // namespace stage
} // namespace dhe
