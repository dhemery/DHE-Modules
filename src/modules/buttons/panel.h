#pragma once

#include "control-ids.h"

#include "controls/buttons.h"
#include "controls/ports.h"

#include "widgets/panel-widget"

#include "rack.hpp"

namespace dhe {

namespace buttons {

struct Panel : public PanelWidget<Panel> {
  static auto constexpr svg_dir = "buttons";
  static auto constexpr svg_name = "buttons";
  static auto constexpr hp = 5;

  Panel(rack::engine::Module *module) : PanelWidget{module} {
    auto constexpr button_x = 5.F;
    auto constexpr negate_x = button_x + 6.5F;
    auto constexpr port_x = negate_x + 7.7F;

    auto constexpr top = 21.F;
    auto constexpr dy = 12.7F;

    for (int i = 0; i < button_count; i++) {
      auto const y = top + static_cast<float>(i) * dy;
      Button::install<Toggle>(this, ParamIds::Button + i, mm2px(button_x, y));
      Button::install<Momentary>(this, ParamIds::Negate + i,
                                 mm2px(negate_x, y));
      OutPort::install(this, OutputIds::Out + i, port_x, y);
    }
  }
};
} // namespace buttons
} // namespace dhe
