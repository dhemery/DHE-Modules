#pragma once

#include "GatorControls.h"
#include "widgets/ControlWidgets.h"
#include "widgets/Screws.h"

#include <app/ModuleWidget.hpp>

namespace dhe {
namespace gator {

class GatorPanel : public rack::app::ModuleWidget {
  using Controls = GatorControls;

public:
  explicit GatorPanel(rack::engine::Module *module) {
    auto constexpr slug = "gator";
    auto constexpr hp = 9;

    setModule(module);
    setPanel(backgroundSvg(slug));
    installScrews(this, hp);

    auto constexpr top = hp2mm(3.5F);
    auto constexpr dy = hp2mm(1.75F);

    auto constexpr left = hp2mm(1.5F);
    auto constexpr lc = hp2mm(3.25F);
    auto constexpr rc = hp2mm(5.75F);
    auto constexpr right = hp2mm(7.5F);

    auto y = top;
    for (auto i = 0; i < Controls::inputCount / 2; i++) {
      addInput(Jack::input(slug, module, left, y, Controls::Inputs + i));
      addParam(
          Toggle::button(slug, module, lc, y, Controls::NegateButtons + i));
      addParam(Toggle::button(slug, module, rc, y,
                              Controls::NegateButtons + i +
                                  Controls::inputCount / 2));
      addInput(Jack::input(slug, module, right, y,
                           Controls::Inputs + i + Controls::inputCount / 2));
      y += dy;
    }

    auto row = top + 8.75 * dy;
    addOutput(Jack::output(slug, module, left, row, Controls::AndOutput));
    addOutput(Jack::output(slug, module, lc, row, Controls::NandOutput));
    addOutput(Jack::output(slug, module, rc, row, Controls::OrOutput));
    addOutput(Jack::output(slug, module, right, row, Controls::NorOutput));

    row += hp2mm(2.75);
    addOutput(Jack::output(slug, module, left, row, Controls::EvenOutput));
    addOutput(Jack::output(slug, module, lc, row, Controls::OddOutput));
    addOutput(Jack::output(slug, module, rc, row, Controls::XorOutput));
    addOutput(Jack::output(slug, module, right, row, Controls::XnorOutput));
  }
};
} // namespace gator
} // namespace dhe
