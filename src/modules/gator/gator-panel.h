#pragma once

#include "gator-controls.h"

#include "controls/buttons.h"
#include "controls/ports.h"
#include "controls/switches.h"
#include "panels/panel-widget.h"

#include "rack.hpp"

namespace dhe {
namespace gator {

struct GatorPanel : public PanelWidget<GatorPanel> {

  static auto constexpr hp = 9;
  static auto constexpr panel_file = "gator";
  static auto constexpr svg_dir = "gator";

public:
  explicit GatorPanel(rack::engine::Module *module)
      : PanelWidget<GatorPanel>{module} {
    using Controls = GatorControls;
    auto constexpr top = hp2mm(3.5F);
    auto constexpr dy = hp2mm(1.75F);

    auto constexpr left = hp2mm(1.5F);
    auto constexpr lc = hp2mm(3.25F);
    auto constexpr rc = hp2mm(5.75F);
    auto constexpr right = hp2mm(7.5F);

    auto y = top;
    for (auto i = 0; i < Controls::InputCount / 2; i++) {
      InPort::install(this, Controls::Inputs + i, left, y);
      Button::install<Toggle>(this, Controls::NegateButtons + i, lc, y);
      Button::install<Toggle>(
          this, Controls::NegateButtons + i + Controls::InputCount / 2, rc, y);
      InPort::install(this, Controls::Inputs + i + Controls::InputCount / 2,
                      right, y);
      y += dy;
    }

    auto row = top + 8.75F * dy;
    OutPort::install(this, Controls::AndOutput, left, row);
    OutPort::install(this, Controls::NandOutput, lc, row);
    OutPort::install(this, Controls::OrOutput, rc, row);
    OutPort::install(this, Controls::NorOutput, right, row);

    row += hp2mm(2.75);
    OutPort::install(this, Controls::EvenOutput, left, row);
    OutPort::install(this, Controls::OddOutput, lc, row);
    OutPort::install(this, Controls::XorOutput, rc, row);
    OutPort::install(this, Controls::XnorOutput, right, row);
  }
};
} // namespace gator
} // namespace dhe
