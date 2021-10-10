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
  using Knob = Knobs<RangerPanel>;
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
    addParam(Knob::medium(module, left, y, Controls::LevelKnob));
    Output::install(this, Controls::RangerOutput, right, y);

    y += dy;
    Input::install(this, Controls::LevelCvInput, left, y);
    addParam(Knob::tiny(module, right, y, Controls::LevelAvKnob));

    y += dy / 2.F;
    y += dy * 0.75F;
    addParam(Knob::medium(module, left, y, Controls::CcwLimitKnob));
    addParam(Knob::medium(module, right, y, Controls::CwLimitKnob));

    y += dy;
    addParam(Knob::tiny(module, left, y, Controls::CcwLimitAvKnob));
    addParam(Knob::tiny(module, right, y, Controls::CwLimitAvKnob));

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
