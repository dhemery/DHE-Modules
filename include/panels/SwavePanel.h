#pragma once

#include "modules/Swave.h"
#include "panel.h"

namespace DHE {

class SwavePanel : public Panel<SwavePanel> {
public:
  explicit SwavePanel(Swave *module);
  static constexpr auto module_slug = "swave";
  static constexpr auto hp = 4;
};

} // namespace DHE
