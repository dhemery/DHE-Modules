#pragma once

#include "blossom-controls.h"
#include "widgets/control-widgets.h"
#include "widgets/screws.h"

#include <app/ModuleWidget.hpp>

namespace dhe {

namespace blossom {

class BlossomPanel : public rack::app::ModuleWidget {
  using Controls = BlossomControls;
  using Jack = dhe::Jack<BlossomPanel>;

public:
  static auto constexpr svg_dir = "blossom";

  explicit BlossomPanel(rack::engine::Module *module) {
    auto constexpr hp = 11;

    setModule(module);
    setPanel(load_svg(svg_dir, "blossom"));
    install_screws(this, hp);

    auto constexpr width = hp2mm(hp);

    auto constexpr column1 = width / 7.F;
    auto constexpr column4 = width - column1;
    auto constexpr column2 = (column4 - column1) / 3.F + column1;
    auto constexpr column3 = width - column2;

    auto y = 25.F;
    auto dy = 18.5F;

    addInput(Jack::input(module, column1, y, Controls::SpeedCvInput));
    addParam(Knob::tiny(svg_dir, module, column2, y, Controls::SpeedAvKNob));
    addParam(Knob::large(svg_dir, module, column3, y, Controls::SpeedKnob));

    y += dy;
    addInput(Jack::input(module, column1, y, Controls::RatioCvInput));
    addParam(Knob::tiny(svg_dir, module, column2, y, Controls::RatioAvKnob));
    addParam(Knob::large(svg_dir, module, column3, y, Controls::RatioKnob));
    addParam(Toggle::thumb(2, svg_dir, module, column4, y,
                           Controls::FreeRatioSwitch));

    y += dy;
    addInput(Jack::input(module, column1, y, Controls::DepthCvInput));
    addParam(Knob::tiny(svg_dir, module, column2, y, Controls::DepthAvKnob));
    addParam(Knob::large(svg_dir, module, column3, y, Controls::DepthKnob));

    y += dy;
    addInput(Jack::input(module, column1, y, Controls::PhaseCvInput));
    addParam(
        Knob::tiny(svg_dir, module, column2, y, Controls::PhaseOffsetAvKnob));
    addParam(
        Knob::large(svg_dir, module, column3, y, Controls::PhaseOffsetKnob));

    y = 97.F;
    dy = 15.F;
    auto constexpr output_port_offset = 1.25F;

    addInput(Jack::input(module, column1, y, Controls::XGainCvInput));
    addParam(Knob::small(svg_dir, module, column2, y, Controls::XGainKnob));
    addParam(
        Toggle::thumb(2, svg_dir, module, column3, y, Controls::XRangeSwitch));
    addOutput(Jack::output(module, column4, y + output_port_offset,
                           Controls::XOutput));

    y += dy;
    addInput(Jack::input(module, column1, y, Controls::YGainCvInput));
    addParam(Knob::small(svg_dir, module, column2, y, Controls::YGainKnob));
    addParam(
        Toggle::thumb(2, svg_dir, module, column3, y, Controls::YRangeSwitch));
    addOutput(Jack::output(module, column4, y + output_port_offset,
                           Controls::YOutput));
  }
};
} // namespace blossom
} // namespace dhe
