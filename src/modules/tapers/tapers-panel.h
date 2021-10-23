#pragma once

#include "tapers-controls.h"

#include "controls/knobs.h"
#include "controls/ports.h"
#include "controls/switches.h"
#include "widgets/screws.h"

#include "rack.hpp"

namespace dhe {
namespace tapers {

class TapersPanel : public rack::app::ModuleWidget {
  using Controls = TapersControls;

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

    InPort::install(this, Controls::LevelCvInput1, column1, y);
    Knob::install<Tiny>(this, Controls::LevelAvKnob1, column2, y);
    Knob::install<Medium>(this, Controls::LevelKnob1, column3, y);
    y += dy;
    InPort::install(this, Controls::CurveCvInput1, column1, y);
    Knob::install<Tiny>(this, Controls::CurveAvKnob1, column2, y);
    Knob::install<Medium>(this, Controls::CurveKnob1, column3, y);
    y += dy;
    ThumbSwitch<2>::install(this, Controls::ShapeSwitch1, column1, y);
    ThumbSwitch<2>::install(this, Controls::LevelRangeSwitch1, column2, y);
    OutPort::install(this, Controls::TaperOutput1, column3, y);

    y += dy + panel_buffer;

    InPort::install(this, Controls::LevelCvInput2, column1, y);
    Knob::install<Tiny>(this, Controls::LevelAvKnob2, column2, y);
    Knob::install<Medium>(this, Controls::LevelKnob2, column3, y);
    y += dy;
    InPort::install(this, Controls::CurveCvInput2, column1, y);
    Knob::install<Tiny>(this, Controls::CurveAvKnob2, column2, y);
    Knob::install<Medium>(this, Controls::CurveKnob2, column3, y);
    y += dy;
    ThumbSwitch<2>::install(this, Controls::ShapeSwitch2, column1, y);
    ThumbSwitch<2>::install(this, Controls::LevelRangeSwitch2, column2, y);
    OutPort::install(this, Controls::TaperOutput2, column3, y);
  }
};
} // namespace tapers
} // namespace dhe
