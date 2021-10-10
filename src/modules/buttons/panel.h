#pragma once

#include "./control-ids.h"
#include "controls/port.h"
#include "widgets/screws.h"
#include "widgets/switches.h"

#include <app/ModuleWidget.hpp>

namespace dhe {

namespace buttons {

class Panel : public rack::app::ModuleWidget {
  using Switch = Switches<Panel>;

public:
  static auto constexpr svg_dir = "buttons";

  Panel(rack::engine::Module *module) {
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
      addParam(Switch::toggle(module, button_x, y, ParamIds::Button + i));
      addParam(Switch::momentary(module, negate_x, y, ParamIds::Negate + i));
      Output::install(this, port_x, y, OutputIds::Out + i);
    }
  }
};
} // namespace buttons
} // namespace dhe
