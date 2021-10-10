#pragma once

#include "./ranger-controls.h"
#include "controls/port.h"
#include "widgets/knobs.h"
#include "widgets/screws.h"
#include "widgets/switches.h"

#include <app/ModuleWidget.hpp>

namespace dhe {
namespace ranger {

class RangerPanel : public rack::app::ModuleWidget {
  using Controls = RangerControls;
  using Switch = Switches<RangerPanel>;

public:
  static auto constexpr svg_dir = "ranger";

  explicit RangerPanel(rack::engine::Module *module) {
    auto constexpr hp = 6;

    setModule(module);
    setPanel(load_svg(svg_dir, "ranger"));
    install_screws(this, hp);

    auto constexpr left = hp2mm(1.5F);
    auto constexpr right = hp2mm(hp) - left;

    auto constexpr dy = 15.F;
    auto y = 14.5F;

    y += dy * 0.75F;
    Knob::install<Medium>(this, Controls::LevelKnob, left, y);
    Output::install(this, Controls::RangerOutput, right, y);

    y += dy;
    Input::install(this, Controls::LevelCvInput, left, y);
    Knob::install<Tiny>(this, Controls::LevelAvKnob, right, y);

    y += dy / 2.F;
    y += dy * 0.75F;
    Knob::install<Medium>(this, Controls::CcwLimitKnob, left, y);
    Knob::install<Medium>(this, Controls::CwLimitKnob, right, y);

    y += dy;
    Knob::install<Tiny>(this, Controls::CcwLimitAvKnob, left, y);
    Knob::install<Tiny>(this, Controls::CwLimitAvKnob, right, y);

    y += dy;
    Input::install(this, Controls::CcwLimitCvInput, left, y);
    Input::install(this, Controls::CwLimitCvInput, right, y);

    y += dy;
    addParam(Switch::thumb<2>(module, left, y, Controls::CcwLimitRangeSwitch));
    addParam(Switch::thumb<2>(module, right, y, Controls::CwLimitRangeSwitch));
  }
};
} // namespace ranger
} // namespace dhe
