#pragma once

#include "Panel.h"
#include "modules/Swave.h"

namespace dhe {

class SwavePanel : public Panel<SwavePanel> {
public:
  explicit SwavePanel(Swave *module);
  static constexpr auto moduleSlug = "swave";
  static constexpr auto hp = 4;
};

} // namespace dhe
