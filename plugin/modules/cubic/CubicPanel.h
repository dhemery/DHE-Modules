#pragma once

#include "../../widgets/Panel.h"
#include "Cubic.h"

namespace dhe {

class CubicPanel : public Panel<CubicPanel> {
public:
  explicit CubicPanel(Cubic *cubic);
  static constexpr auto moduleSlug = "cubic";
  static constexpr auto hp = 5;
};
} // namespace dhe
