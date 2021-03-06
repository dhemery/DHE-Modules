#pragma once

#include "tapers-controls.h"
#include "widgets/control-widgets.h"
#include "widgets/screws.h"

#include <app/ModuleWidget.hpp>

namespace dhe {
namespace tapers {

class TapersPanel : public rack::app::ModuleWidget {
  using Controls = TapersControls;

public:
  explicit TapersPanel(rack::engine::Module *module) {
    auto constexpr svg_dir = "tapers";
    auto constexpr hp = 9;

    setModule(module);
    setPanel(load_svg(svg_dir, "tapers"));
    install_screws(this, hp);

    auto constexpr width = hp2mm(hp);

    auto constexpr column1 = width / 5.F + 0.333333333F;
    auto constexpr column2 = width / 2.F;
    auto constexpr column3 = width - column1;

    auto constexpr dy = 16.F;
    auto constexpr panel_buffer = 4.F;
    auto y = 24.F;

    addInput(Jack::input(svg_dir, module, column1, y, Controls::LevelCvInput1));
    addParam(Knob::tiny(svg_dir, module, column2, y, Controls::LevelAvKnob1));
    addParam(Knob::medium(svg_dir, module, column3, y, Controls::LevelKnob1));
    y += dy;
    addInput(Jack::input(svg_dir, module, column1, y, Controls::CurveCvInput1));
    addParam(Knob::tiny(svg_dir, module, column2, y, Controls::CurveAvKnob1));
    addParam(Knob::medium(svg_dir, module, column3, y, Controls::CurveKnob1));
    y += dy;
    addParam(
        Toggle::thumb(2, svg_dir, module, column1, y, Controls::ShapeSwitch1));
    addParam(Toggle::thumb(2, svg_dir, module, column2, y,
                           Controls::LevelRangeSwitch1));
    addOutput(
        Jack::output(svg_dir, module, column3, y, Controls::TaperOutput1));

    y += dy + panel_buffer;

    addInput(Jack::input(svg_dir, module, column1, y, Controls::LevelCvInput2));
    addParam(Knob::tiny(svg_dir, module, column2, y, Controls::LevelAvKnob2));
    addParam(Knob::medium(svg_dir, module, column3, y, Controls::LevelKnob2));
    y += dy;
    addInput(Jack::input(svg_dir, module, column1, y, Controls::CurveCvInput2));
    addParam(Knob::tiny(svg_dir, module, column2, y, Controls::CurveAvKnob2));
    addParam(Knob::medium(svg_dir, module, column3, y, Controls::CurveKnob2));
    y += dy;
    addParam(
        Toggle::thumb(2, svg_dir, module, column1, y, Controls::ShapeSwitch2));
    addParam(Toggle::thumb(2, svg_dir, module, column2, y,
                           Controls::LevelRangeSwitch2));
    addOutput(
        Jack::output(svg_dir, module, column3, y, Controls::TaperOutput2));
  }
};
} // namespace tapers
} // namespace dhe
