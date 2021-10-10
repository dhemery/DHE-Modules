#pragma once

#include "./tapers-controls.h"
#include "controls/port.h"
#include "widgets/knobs.h"
#include "widgets/screws.h"
#include "widgets/switches.h"

#include <app/ModuleWidget.hpp>

namespace dhe {
namespace tapers {

class TapersPanel : public rack::app::ModuleWidget {
  using Controls = TapersControls;
  using Knob = Knobs<TapersPanel>;
  using Switch = Switches<TapersPanel>;

public:
  static auto constexpr svg_dir = "tapers";

  explicit TapersPanel(rack::engine::Module *module) {
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

    Input::install(this, Controls::LevelCvInput1, column1, y);
    addParam(Knob::tiny(module, column2, y, Controls::LevelAvKnob1));
    addParam(Knob::medium(module, column3, y, Controls::LevelKnob1));
    y += dy;
    Input::install(this, Controls::CurveCvInput1, column1, y);
    addParam(Knob::tiny(module, column2, y, Controls::CurveAvKnob1));
    addParam(Knob::medium(module, column3, y, Controls::CurveKnob1));
    y += dy;
    addParam(Switch::thumb<2>(module, column1, y, Controls::ShapeSwitch1));
    addParam(Switch::thumb<2>(module, column2, y, Controls::LevelRangeSwitch1));
    Output::install(this, Controls::TaperOutput1, column3, y);

    y += dy + panel_buffer;

    Input::install(this, Controls::LevelCvInput2, column1, y);
    addParam(Knob::tiny(module, column2, y, Controls::LevelAvKnob2));
    addParam(Knob::medium(module, column3, y, Controls::LevelKnob2));
    y += dy;
    Input::install(this, Controls::CurveCvInput2, column1, y);
    addParam(Knob::tiny(module, column2, y, Controls::CurveAvKnob2));
    addParam(Knob::medium(module, column3, y, Controls::CurveKnob2));
    y += dy;
    addParam(Switch::thumb<2>(module, column1, y, Controls::ShapeSwitch2));
    addParam(Switch::thumb<2>(module, column2, y, Controls::LevelRangeSwitch2));
    Output::install(this, Controls::TaperOutput2, column3, y);
  }
};
} // namespace tapers
} // namespace dhe
