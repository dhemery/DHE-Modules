#pragma once

#include "Panel.h"
#include "modules/Cubic.h"

namespace dhe {

class CubicPanel : public Panel<CubicPanel> {
public:
  explicit CubicPanel(Cubic *cubic);
  static constexpr auto moduleSlug = "cubic";
  static constexpr auto hp = 5;
};
} // namespace dhe
