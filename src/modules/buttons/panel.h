#pragma once

#include "control-ids.h"
#include "widgets/control-widgets.h"
#include "widgets/screws.h"

#include <app/ModuleWidget.hpp>

namespace dhe {

namespace buttons {

class Panel : public rack::app::ModuleWidget {
  using Param = ParamIds;
  using Output = OutputIds;

public:
  Panel(rack::engine::Module *module) {
    auto constexpr svg_dir = "buttons";
    auto constexpr hp = 5;

    setModule(module);
    setPanel(load_svg(svg_dir, "buttons"));
    install_screws(this, hp);

    auto constexpr button_x = 5.F;
    auto constexpr negate_x = button_x + 6.5F;
    auto constexpr port_x = negate_x + 7.7F;

    auto constexpr top = 21.F;
    auto constexpr dy = 12.7F;

    for (int i = 0; i < button_count; i++) {
      auto const y = top + static_cast<float>(i) * dy;
      addParam(Button::toggle(svg_dir, module, button_x, y, Param::Button + i));
      addParam(
          Button::momentary(svg_dir, module, negate_x, y, Param::Negate + i));
      addOutput(Jack::output(svg_dir, module, port_x, y, Output::Out + i));
    }
  }
};
} // namespace buttons
} // namespace dhe
