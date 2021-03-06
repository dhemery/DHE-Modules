#pragma once

#include "gator-controls.h"
#include "widgets/control-widgets.h"
#include "widgets/screws.h"

#include <app/ModuleWidget.hpp>

namespace dhe {
namespace gator {

class GatorPanel : public rack::app::ModuleWidget {
  using Controls = GatorControls;

public:
  explicit GatorPanel(rack::engine::Module *module) {
    auto constexpr svg_dir = "gator";
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
      addInput(Jack::input(svg_dir, module, left, y, Controls::Inputs + i));
      addParam(
          Button::toggle(svg_dir, module, lc, y, Controls::NegateButtons + i));
      addParam(Button::toggle(svg_dir, module, rc, y,
                              Controls::NegateButtons + i +
                                  Controls::InputCount / 2));
      addInput(Jack::input(svg_dir, module, right, y,
                           Controls::Inputs + i + Controls::InputCount / 2));
      y += dy;
    }

    auto row = top + 8.75 * dy;
    addOutput(Jack::output(svg_dir, module, left, row, Controls::AndOutput));
    addOutput(Jack::output(svg_dir, module, lc, row, Controls::NandOutput));
    addOutput(Jack::output(svg_dir, module, rc, row, Controls::OrOutput));
    addOutput(Jack::output(svg_dir, module, right, row, Controls::NorOutput));

    row += hp2mm(2.75);
    addOutput(Jack::output(svg_dir, module, left, row, Controls::EvenOutput));
    addOutput(Jack::output(svg_dir, module, lc, row, Controls::OddOutput));
    addOutput(Jack::output(svg_dir, module, rc, row, Controls::XorOutput));
    addOutput(Jack::output(svg_dir, module, right, row, Controls::XnorOutput));
  }
};
} // namespace gator
} // namespace dhe
