#pragma once

#include "booster-stage-controls.h"

#include <app/ModuleWidget.hpp>

namespace dhe {
namespace stage {

class BoosterStagePanel : public rack::app::ModuleWidget {
  using Controls = BoosterStageControls;
  using Jack = Jacks<BoosterStagePanel>;
  using Knob = Knobs<BoosterStagePanel>;
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

    addInput(Jack::input(module, column1, y, Controls::LevelCvInput));
    addParam(Knob::large(module, column3, y, Controls::LevelKnob));

    addParam(Switch::thumb<2>(module, column5, y, Controls::LevelRangeSwitch));

    y += dy;
    addInput(Jack::input(module, column1, y, Controls::CurveCvInput));
    addParam(Knob::large(module, column3, y, Controls::CurveKnob));
    addParam(Switch::thumb<2>(module, column5, y, Controls::ShapeSwitch));

    y += dy;
    addInput(Jack::input(module, column1, y, Controls::DurationCvInput));
    addParam(Knob::large(module, column3, y, Controls::DurationKnob));

    addParam(
        Switch::thumb<3>(module, column5, y, Controls::DurationRangeSwitch));

    y = 82.F;
    dy = 15.F;

    addInput(Jack::input(module, column1, y, Controls::DeferInput));
    addParam(Switch::momentary(module, column2, y, Controls::DeferButton));

    addParam(Switch::momentary<OutputButton>(module, column4, y,
                                             Controls::ActiveButton));
    addOutput(Jack::output(module, column5, y, Controls::ActiveOutput));

    y += dy;
    addInput(Jack::input(module, column1, y, Controls::TriggerInput));
    addParam(Switch::momentary(module, column2, y, Controls::TriggerButton));
    addParam(Switch::momentary<OutputButton>(module, column4, y,
                                             Controls::EocButton));
    addOutput(Jack::output(module, column5, y, Controls::EocOutput));

    y += dy;
    addInput(Jack::input(module, column1, y, Controls::EnvelopeInput));
    addOutput(Jack::output(module, column5, y, Controls::EnvelopeOutput));
  }
};

} // namespace stage
} // namespace dhe
