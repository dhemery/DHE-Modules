#pragma once

#include "control-ids.h"

#include "controls/ports.h"
#include "controls/switches.h"
#include "widgets/panel-widget.h"

#include "rack.hpp"

namespace dhe {
namespace gator {

struct Panel : public PanelWidget<Panel> {
  static auto constexpr hp = 9;
  static auto constexpr panel_file = "gator";
  static auto constexpr svg_dir = "gator";

  explicit Panel(rack::engine::Module *module) : PanelWidget<Panel>{module} {
    auto constexpr top = hp2mm(3.5F);
    auto constexpr dy = hp2mm(1.75F);

    auto constexpr left = hp2mm(1.5F);
    auto constexpr lc = hp2mm(3.25F);
    auto constexpr rc = hp2mm(5.75F);
    auto constexpr right = hp2mm(7.5F);

    auto y = top;
    for (auto i = 0; i < Input::Count / 2; i++) {
      InPort::install(this, Input::Signal + i, left, y);
      Button::install<Toggle>(this, Param::NegateSignal + i, lc, y);
      Button::install<Toggle>(this, Param::NegateSignal + i + Input::Count / 2,
                              rc, y);
      InPort::install(this, Input::Signal + i + Input::Count / 2, right, y);
      y += dy;
    }

    y = top + 8.75F * dy;
    OutPort::install(this, Output::And, left, y);
    OutPort::install(this, Output::Nand, lc, y);
    OutPort::install(this, Output::Or, rc, y);
    OutPort::install(this, Output::Nor, right, y);

    y += hp2mm(2.75);
    OutPort::install(this, Output::Even, left, y);
    OutPort::install(this, Output::Odd, lc, y);
    OutPort::install(this, Output::Xor, rc, y);
    OutPort::install(this, Output::Xnor, right, y);
  }
};
} // namespace gator
} // namespace dhe
