#pragma once

#include "Cubic.h"
#include "widgets/Panel.h"

namespace dhe {

class CubicPanel : public Panel<CubicPanel> {
public:
  explicit CubicPanel(rack::engine::Module *module);
  static constexpr auto moduleSlug = "cubic";
  static constexpr auto hp = 5;
};
} // namespace dhe
