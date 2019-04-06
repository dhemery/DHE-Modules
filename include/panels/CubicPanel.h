#pragma once

#include <display/panel.h>
#include <modules/Cubic.h>

namespace DHE {

class CubicPanel : public Panel<CubicPanel> {
public:
  explicit CubicPanel(Cubic *cubic);
  static constexpr auto module_slug = "cubic";
  static constexpr auto hp = 5;
};
}