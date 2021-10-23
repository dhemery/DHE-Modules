#pragma once

#include "booster-stage-controls.h"

#include "controls/knobs.h"
#include "controls/ports.h"
#include "controls/switches.h"
#include "panels/screws.h"

#include "rack.hpp"

namespace dhe {
namespace stage {

class BoosterStagePanel : public rack::app::ModuleWidget {
  using Controls = BoosterStageControls;

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

    InPort::install(this, Controls::LevelCvInput, column1, y);
    Knob::install<Large>(this, Controls::LevelKnob, column3, y);

    ThumbSwitch<2>::install(this, Controls::LevelRangeSwitch, column5, y);

    y += dy;
    InPort::install(this, Controls::CurveCvInput, column1, y);
    Knob::install<Large>(this, Controls::CurveKnob, column3, y);
    ThumbSwitch<2>::install(this, Controls::ShapeSwitch, column5, y);

    y += dy;
    InPort::install(this, Controls::DurationCvInput, column1, y);
    Knob::install<Large>(this, Controls::DurationKnob, column3, y);

    ThumbSwitch<3>::install(this, Controls::DurationRangeSwitch, column5, y);

    y = 82.F;
    dy = 15.F;

    InPort::install(this, Controls::DeferInput, column1, y);
    Button::install<Momentary>(this, Controls::DeferButton, column2, y);

    Button::install<Momentary, Reversed>(this, Controls::ActiveButton, column4,
                                         y);
    OutPort::install(this, Controls::ActiveOutput, column5, y);

    y += dy;
    InPort::install(this, Controls::TriggerInput, column1, y);
    Button::install<Momentary>(this, Controls::TriggerButton, column2, y);
    Button::install<Momentary, Reversed>(this, Controls::EocButton, column4, y);
    OutPort::install(this, Controls::EocOutput, column5, y);

    y += dy;
    InPort::install(this, Controls::EnvelopeInput, column1, y);
    OutPort::install(this, Controls::EnvelopeOutput, column5, y);
  }
};

} // namespace stage
} // namespace dhe
