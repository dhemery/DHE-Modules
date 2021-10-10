#pragma once

#include "./gator-controls.h"
#include "controls/port.h"
#include "widgets/screws.h"
#include "widgets/switches.h"

#include <app/ModuleWidget.hpp>

namespace dhe {
namespace gator {

class GatorPanel : public rack::app::ModuleWidget {
  using Controls = GatorControls;
  using Switch = Switches<GatorPanel>;

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
      Input::install(this, left, y, Controls::Inputs + i);
      addParam(Switch::toggle(module, lc, y, Controls::NegateButtons + i));
      addParam(Switch::toggle(module, rc, y,
                              Controls::NegateButtons + i +
                                  Controls::InputCount / 2));
      Input::install(this, right, y,
                     Controls::Inputs + i + Controls::InputCount / 2);
      y += dy;
    }

    auto row = top + 8.75F * dy;
    Output::install(this, left, row, Controls::AndOutput);
    Output::install(this, lc, row, Controls::NandOutput);
    Output::install(this, rc, row, Controls::OrOutput);
    Output::install(this, right, row, Controls::NorOutput);

    row += hp2mm(2.75);
    Output::install(this, left, row, Controls::EvenOutput);
    Output::install(this, lc, row, Controls::OddOutput);
    Output::install(this, rc, row, Controls::XorOutput);
    Output::install(this, right, row, Controls::XnorOutput);
  }
};
} // namespace gator
} // namespace dhe
