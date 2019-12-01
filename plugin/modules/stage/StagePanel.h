#pragma once

#include "StageControls.h"
#include "widgets/Panel.h"

namespace dhe {
namespace stage {

  class StagePanel : public Panel<StagePanel> {
    using Controls = StageControls;
    using MainKnob = LargeKnob<StagePanel>;

  public:
    explicit StagePanel(rack::engine::Module *module) {
      setModule(module);
      setPanel(backgroundSvg<StagePanel>());
      installScrews(this);

      auto width = hp2mm(hp);

      auto column1 = width / 4.F + 0.333333F;
      auto column2 = width / 2.F;
      auto column3 = width - column1;

      auto y = 25.F;
      auto dy = 18.5F;

      install<LargeKnob>(this, module, column2, y, Controls::LevelKnob);

      y += dy;
      install<LargeKnob>(this, module, column2, y, Controls::CurveKnob);

      y += dy;
      install<LargeKnob>(this, module, column2, y, Controls::DurationKnob);

      y = 82.F;
      dy = 15.F;

      installInput(this, module, column1, y, Controls::DeferInput);
      installOutput(this, module, column3, y, Controls::ActiveOutput);

      y += dy;
      installInput(this, module, column1, y, Controls::TriggerInput);
      installOutput(this, module, column3, y, Controls::EocOutput);

      y += dy;
      installInput(this, module, column1, y, Controls::EnvelopeInput);
      installOutput(this, module, column3, y, Controls::EnvelopeOutput);
    }
    static constexpr auto moduleSlug = "stage";
    static constexpr auto hp = 5;
  };
} // namespace stage
} // namespace dhe
