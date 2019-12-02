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

      addParam(Toggle::stepper(2, moduleSlug, module, column2, y, Controls::ModeSwitch));

      y += dy;
      addInput(Jack::input(moduleSlug, module, column1, y, Controls::DurationCvInput));
      addParam(Toggle::stepper(3, moduleSlug, module, column3, y, Controls::DurationRangeSwitch));

      y += dy;
      addParam(Knob::large(moduleSlug, module, column2, y, Controls::DurationKnob));

      y = 82.F;
      dy = 15.F;

      addInput(Jack::input(moduleSlug, module, column1, y, Controls::DeferInput));
      addOutput(Jack::output(moduleSlug, module, column3, y, Controls::ActiveOutput));

      y += dy;
      addInput(Jack::input(moduleSlug, module, column1, y, Controls::TriggerInput));
      addOutput(Jack::output(moduleSlug, module, column3, y, Controls::EocOutput));

      y += dy;
      addInput(Jack::input(moduleSlug, module, column1, y, Controls::EnvelopeInput));
      addOutput(Jack::output(moduleSlug, module, column3, y, Controls::EnvelopeOutput));
    }
    static constexpr auto moduleSlug = "hostage";
  };
} // namespace stage
} // namespace dhe
