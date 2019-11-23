#pragma once

#include "BoosterStageControls.h"
#include "widgets/Panel.h"

namespace dhe {
namespace stage {

  class BoosterStagePanel : public Panel<BoosterStagePanel> {
    using Controls = BoosterStageControls;

  public:
    explicit BoosterStagePanel(rack::engine::Module *module) : Panel{module, hp} {
      auto widgetRightEdge = width();

      auto column1 = widgetRightEdge / 6.F + 0.3333333F;
      auto column3 = widgetRightEdge / 2.F;
      auto column5 = widgetRightEdge - column1;
      auto buttonPortDistance = 7.891F;
      auto column2 = column1 + buttonPortDistance;
      auto column4 = column5 - buttonPortDistance;

      auto y = 25.F;
      auto dy = 18.5F;

      input(column1, y, Controls::LevelCvInput);
      knob<LargeKnob>(column3, y, Controls::LevelKnob);

      toggle<2>(column5, y, Controls::LevelRangeSwitch);

      y += dy;
      input(column1, y, Controls::CurveCvInput);
      knob<LargeKnob>(column3, y, Controls::CurveKnob);
      toggle<2>(column5, y, Controls::ShapeSwitch);

      y += dy;
      input(column1, y, Controls::DurationCvInput);
      knob<LargeKnob>(column3, y, Controls::DurationKnob);

      toggle<3>(column5, y, Controls::DurationRangeSwitch);

      y = 82.F;
      dy = 15.F;

      input(column1, y, Controls::DeferInput);
      button(column2, y, Controls::DeferButton);

      button<ReverseButton>(column4, y, Controls::ActiveButton);
      output(column5, y, Controls::ActiveOutput);

      y += dy;
      input(column1, y, Controls::TriggerInput);
      button(column2, y, Controls::TriggerButton);
      button<ReverseButton>(column4, y, Controls::EocButton);
      output(column5, y, Controls::EocOutput);

      y += dy;
      input(column1, y, Controls::EnvelopeInput);
      output(column5, y, Controls::EnvelopeOutput);
    }
    static constexpr auto moduleSlug = "booster-stage";
    static constexpr auto hp = 8;
  };

} // namespace stage
} // namespace dhe
