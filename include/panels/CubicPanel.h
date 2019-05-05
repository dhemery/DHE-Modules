#pragma once

#include "modules/Cubic.h"
#include "panel.h"

namespace DHE {

class CubicPanel : public Panel<CubicPanel> {
public:
  explicit CubicPanel(Cubic *cubic);
  static constexpr auto module_slug = "cubic";
  static constexpr auto hp = 5;
};
} // namespace DHE
