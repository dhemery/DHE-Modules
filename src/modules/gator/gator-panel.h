#pragma once

#include "./gator-controls.h"
#include "controls/ports.h"
#include "controls/switches.h"
#include "widgets/screws.h"

#include <app/ModuleWidget.hpp>

namespace dhe {
namespace gator {

class GatorPanel : public rack::app::ModuleWidget {
  using Controls = GatorControls;

public:
  static auto constexpr svg_dir = "gator";

  explicit GatorPanel(rack::engine::Module *module) {
    auto constexpr hp = 9;

    setModule(module);
    setPanel(load_svg(svg_dir, "gator"));
    install_screws(this, hp);

    auto constexpr top = hp2mm(3.5F);
    auto constexpr dy = hp2mm(1.75F);

    auto constexpr left = hp2mm(1.5F);
    auto constexpr lc = hp2mm(3.25F);
    auto constexpr rc = hp2mm(5.75F);
    auto constexpr right = hp2mm(7.5F);

    auto y = top;
    for (auto i = 0; i < Controls::InputCount / 2; i++) {
      Input::install(this, Controls::Inputs + i, left, y);
      Button::install<Toggle>(this, Controls::NegateButtons + i, lc, y);
      Button::install<Toggle>(
          this, Controls::NegateButtons + i + Controls::InputCount / 2, rc, y);
      Input::install(this, Controls::Inputs + i + Controls::InputCount / 2,
                     right, y);
      y += dy;
    }

    auto row = top + 8.75F * dy;
    Output::install(this, Controls::AndOutput, left, row);
    Output::install(this, Controls::NandOutput, lc, row);
    Output::install(this, Controls::OrOutput, rc, row);
    Output::install(this, Controls::NorOutput, right, row);

    row += hp2mm(2.75);
    Output::install(this, Controls::EvenOutput, left, row);
    Output::install(this, Controls::OddOutput, lc, row);
    Output::install(this, Controls::XorOutput, rc, row);
    Output::install(this, Controls::XnorOutput, right, row);
  }
};
} // namespace gator
} // namespace dhe
