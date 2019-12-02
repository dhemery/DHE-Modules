#pragma once

#include "UpstageControls.h"

#include <app/ModuleWidget.hpp>

namespace dhe {
namespace stage {
  class UpstagePanel : public rack::app::ModuleWidget {
    using Controls = UpstageControls;

  public:
    UpstagePanel(rack::engine::Module *module) {
      auto constexpr slug = "upstage";
      auto constexpr hp = 5;

      setModule(module);
      setPanel(backgroundSvg(slug));
      installScrews(this, hp);

      auto constexpr width = hp2mm(hp);

      auto constexpr column1 = width / 4.F + 0.333333333F;
      auto constexpr column2 = width / 2.F;
      auto constexpr column3 = width - column1;

      auto y = 25.F;
      auto dy = 18.5F;

      addParam(Knob::large(slug, module, column2, y, Controls::LevelKnob));

      y += dy;
      addInput(Jack::input(slug, module, column1, y, Controls::LevelCvInput));
      addParam(Toggle::stepper(2, slug, module, column3, y, Controls::LevelRangeSwitch));

      y += dy;
      addParam(Button::momentary(slug, module, column1, y, Controls::WaitButton));
      addParam(Button::momentary(slug, module, column3, y, Controls::TriggerButton));

      y = 82.F;
      dy = 15.F;

      addInput(Jack::input(slug, module, column1, y, Controls::WaitInput));

      y += dy;
      addInput(Jack::input(slug, module, column1, y, Controls::TriggerInput));
      addOutput(Jack::output(slug, module, column3, y, Controls::TriggerOutput));

      y += dy;
      addOutput(Jack::output(slug, module, column3, y, Controls::EnvelopeOutput));
    }
  };
} // namespace stage

} // namespace dhe
