#pragma once

#include "Panel.h"
#include "modules/Flogic.h"

namespace dhe {

class FlogicPanel : public Panel<FlogicPanel> {
public:
  explicit FlogicPanel(Flogic *flogic);
  static constexpr auto moduleSlug = "flogic";
  static constexpr auto hp = 9;
};
} // namespace dhe
