#pragma once

#include "HostageControls.h"
#include "widgets/Panel.h"

namespace dhe {
namespace stage {
  class HostagePanel : public Panel<HostagePanel> {
    using Controls = HostageControls;

  public:
    HostagePanel(rack::engine::Module *module) : Panel{module, hp} {
      auto widgetRightEdge = width();

      auto column1 = width() / 4.F + 0.333333F;
      auto column2 = widgetRightEdge / 2.F;
      auto column3 = widgetRightEdge - column1;

      auto y = 25.F;
      auto dy = 18.5F;

      toggle<2>(column2, y, Controls::ModeSwitch);

      y += dy;
      input(column1, y, Controls::DurationCvInput);
      toggle<3>(column3, y, Controls::DurationRangeSwitch);

      y += dy;
      knob<LargeKnob>(column2, y, Controls::DurationKnob);

      y = 82.F;
      dy = 15.F;

      input(column1, y, Controls::DeferInput);
      output(column3, y, Controls::ActiveOutput);

      y += dy;
      input(column1, y, Controls::TriggerInput);
      output(column3, y, Controls::EocOutput);

      y += dy;
      input(column1, y, Controls::EnvelopeInput);
      output(column3, y, Controls::EnvelopeOutput);
    }
    static constexpr auto moduleSlug = "hostage";
    static constexpr auto hp = 5;
  };
} // namespace stage
} // namespace dhe
