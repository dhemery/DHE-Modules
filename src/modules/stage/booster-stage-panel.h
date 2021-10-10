#pragma once

#include "./booster-stage-controls.h"
#include "controls/port.h"
#include "widgets/knobs.h"
#include "widgets/screws.h"
#include "widgets/switches.h"

#include <app/ModuleWidget.hpp>

namespace dhe {
namespace stage {

class BoosterStagePanel : public rack::app::ModuleWidget {
  using Controls = BoosterStageControls;
  using Switch = Switches<BoosterStagePanel>;

public:
  static auto constexpr svg_dir = "booster-stage";

  explicit BoosterStagePanel(rack::engine::Module *module) {
    auto constexpr hp = 8;

    setModule(module);
    setPanel(load_svg(svg_dir, "booster-stage"));
    install_screws(this, hp);

    auto constexpr width = hp2mm(hp);

    auto constexpr column1 = width / 6.F + 0.3333333F;
    auto constexpr column3 = width / 2.F;
    auto constexpr column5 = width - column1;
    auto constexpr button_port_distance = 7.891F;
    auto constexpr column2 = column1 + button_port_distance;
    auto constexpr column4 = column5 - button_port_distance;

    auto dy = 18.5F;
    auto y = 25.F;

    Input::install(this, Controls::LevelCvInput, column1, y);
    Knob::install<Large>(this, Controls::LevelKnob, column3, y);

    addParam(Switch::thumb<2>(module, column5, y, Controls::LevelRangeSwitch));

    y += dy;
    Input::install(this, Controls::CurveCvInput, column1, y);
    Knob::install<Large>(this, Controls::CurveKnob, column3, y);
    addParam(Switch::thumb<2>(module, column5, y, Controls::ShapeSwitch));

    y += dy;
    Input::install(this, Controls::DurationCvInput, column1, y);
    Knob::install<Large>(this, Controls::DurationKnob, column3, y);

    addParam(
        Switch::thumb<3>(module, column5, y, Controls::DurationRangeSwitch));

    y = 82.F;
    dy = 15.F;

    Input::install(this, Controls::DeferInput, column1, y);
    addParam(Switch::momentary(module, column2, y, Controls::DeferButton));

    addParam(Switch::momentary<OutputButton>(module, column4, y,
                                             Controls::ActiveButton));
    Output::install(this, Controls::ActiveOutput, column5, y);

    y += dy;
    Input::install(this, Controls::TriggerInput, column1, y);
    addParam(Switch::momentary(module, column2, y, Controls::TriggerButton));
    addParam(Switch::momentary<OutputButton>(module, column4, y,
                                             Controls::EocButton));
    Output::install(this, Controls::EocOutput, column5, y);

    y += dy;
    Input::install(this, Controls::EnvelopeInput, column1, y);
    Output::install(this, Controls::EnvelopeOutput, column5, y);
  }
};

} // namespace stage
} // namespace dhe
