#pragma once

#include "./stage-controls.h"
#include "controls/port.h"
#include "widgets/knobs.h"
#include "widgets/screws.h"

#include <app/ModuleWidget.hpp>

namespace dhe {
namespace stage {

class StagePanel : public rack::app::ModuleWidget {
  using Controls = StageControls;
  using Knob = Knobs<StagePanel>;

public:
  static auto constexpr svg_dir = "stage";

  explicit StagePanel(rack::engine::Module *module) {
    auto constexpr hp = 5;

    setModule(module);
    setPanel(load_svg(svg_dir, "stage"));
    install_screws(this, hp);

    auto constexpr width = hp2mm(hp);

    auto constexpr column1 = width / 4.F + 0.333333F;
    auto constexpr column2 = width / 2.F;
    auto constexpr column3 = width - column1;

    auto y = 25.F;
    auto dy = 18.5F;

    addParam(Knob::large(module, column2, y, Controls::LevelKnob));

    y += dy;
    addParam(Knob::large(module, column2, y, Controls::CurveKnob));

    y += dy;
    addParam(Knob::large(module, column2, y, Controls::DurationKnob));

    y = 82.F;
    dy = 15.F;

    Input::install(this, y, Controls::DeferInput, column1);
    Output::install(this, y, Controls::ActiveOutput, column3);

    y += dy;
    Input::install(this, y, Controls::TriggerInput, column1);
    Output::install(this, y, Controls::EocOutput, column3);

    y += dy;
    Input::install(this, y, Controls::EnvelopeInput, column1);
    Output::install(this, y, Controls::EnvelopeOutput, column3);
  }
};
} // namespace stage
} // namespace dhe
