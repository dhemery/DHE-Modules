#pragma once

#include "blossom-controls.h"
#include "controls/port.h"
#include "widgets/knobs.h"
#include "widgets/screws.h"
#include "widgets/switches.h"

#include <app/ModuleWidget.hpp>

namespace dhe {

namespace blossom {

class BlossomPanel : public rack::app::ModuleWidget {
  using Controls = BlossomControls;
  using Knob = Knobs<BlossomPanel>;
  using Switch = Switches<BlossomPanel>;

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

    Input::install(this, column1, y, Controls::SpeedCvInput);
    addParam(Knob::tiny(module, column2, y, Controls::SpeedAvKNob));
    addParam(Knob::large(module, column3, y, Controls::SpeedKnob));

    y += dy;
    Input::install(this, column1, y, Controls::RatioCvInput);
    addParam(Knob::tiny(module, column2, y, Controls::RatioAvKnob));
    addParam(Knob::large(module, column3, y, Controls::RatioKnob));
    addParam(Switch::thumb<2>(module, column4, y, Controls::FreeRatioSwitch));

    y += dy;
    Input::install(this, column1, y, Controls::DepthCvInput);
    addParam(Knob::tiny(module, column2, y, Controls::DepthAvKnob));
    addParam(Knob::large(module, column3, y, Controls::DepthKnob));

    y += dy;
    Input::install(this, column1, y, Controls::PhaseCvInput);
    addParam(Knob::tiny(module, column2, y, Controls::PhaseOffsetAvKnob));
    addParam(Knob::large(module, column3, y, Controls::PhaseOffsetKnob));

    y = 97.F;
    dy = 15.F;
    auto constexpr output_port_offset = 1.25F;

    Input::install(this, column1, y, Controls::XGainCvInput);
    addParam(Knob::small(module, column2, y, Controls::XGainKnob));
    addParam(Switch::thumb<2>(module, column3, y, Controls::XRangeSwitch));
    Output::install(this, column4, y + output_port_offset, Controls::XOutput);

    y += dy;
    Output::install(this, column1, y, Controls::YGainCvInput);
    addParam(Knob::small(module, column2, y, Controls::YGainKnob));
    addParam(Switch::thumb<2>(module, column3, y, Controls::YRangeSwitch));
    Output::install(this, column4, y + output_port_offset, Controls::YOutput);
  }
};
} // namespace blossom
} // namespace dhe
