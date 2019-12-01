#pragma once

#include "UpstageControls.h"

#include <app/ModuleWidget.hpp>

namespace dhe {
namespace stage {
  class UpstagePanel : public rack::app::ModuleWidget {
    using Controls = UpstageControls;
    using MainKnob = LargeKnob<UpstagePanel>;

  public:
    UpstagePanel(rack::engine::Module *module) {
      auto constexpr hp = 5;

      setModule(module);
      setPanel(backgroundSvg<UpstagePanel>());
      installScrews(this, hp);

      auto width = hp2mm(hp);

      auto column1 = width / 4.F + 0.333333333F;
      auto column2 = width / 2.F;
      auto column3 = width - column1;

      auto y = 25.F;
      auto dy = 18.5F;

      install<LargeKnob>(this, module, column2, y, Controls::LevelKnob);

      y += dy;
      installInput(this, module, column1, y, Controls::LevelCvInput);
      install<Toggle, 2>(this, module, column3, y, Controls::LevelRangeSwitch);

      y += dy;
      install<MomentaryButton>(this, module, column1, y, Controls::WaitButton);
      install<MomentaryButton>(this, module, column3, y, Controls::TriggerButton);

      y = 82.F;
      dy = 15.F;

      installInput(this, module, column1, y, Controls::WaitInput);

      y += dy;
      installInput(this, module, column1, y, Controls::TriggerInput);
      installOutput(this, module, column3, y, Controls::TriggerOutput);

      y += dy;
      installOutput(this, module, column3, y, Controls::EnvelopeOutput);
    }
    static constexpr auto moduleSlug = "upstage";
  };
} // namespace stage

} // namespace dhe
