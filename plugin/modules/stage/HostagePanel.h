#pragma once

#include "HostageControls.h"

#include <app/ModuleWidget.hpp>

namespace dhe {
namespace stage {
  class HostagePanel : public rack::app::ModuleWidget {
    using Controls = HostageControls;

  public:
    HostagePanel(rack::engine::Module *module) {
      auto constexpr hp = 5;

      setModule(module);
      setPanel(backgroundSvg(moduleSlug));
      installScrews(this, hp);

      auto width = hp2mm(hp);

      auto column1 = width / 4.F + 0.333333F;
      auto column2 = width / 2.F;
      auto column3 = width - column1;

      auto y = 25.F;
      auto dy = 18.5F;

      install<Toggle<2>>(this, module, column2, y, Controls::ModeSwitch);

      y += dy;
      installInput(this, module, column1, y, Controls::DurationCvInput);
      install<Toggle<3>>(this, module, column3, y, Controls::DurationRangeSwitch);

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
    static constexpr auto moduleSlug = "hostage";
  };
} // namespace stage
} // namespace dhe
