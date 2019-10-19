#pragma once

#include "Cubic.h"
#include "modules/Panel.h"

namespace dhe {

class CubicPanel : public Panel<CubicPanel> {
public:
  explicit CubicPanel(Cubic *cubic);
  static constexpr auto moduleSlug = "cubic";
  static constexpr auto hp = 5;
};
} // namespace dhe
