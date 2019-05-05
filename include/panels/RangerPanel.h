#pragma once

#include "modules/Ranger.h"
#include "panel.h"

namespace DHE {

class RangerPanel : public Panel<RangerPanel> {
public:
  explicit RangerPanel(Ranger *module);
  static constexpr auto module_slug = "ranger";
  static constexpr auto hp = 6;
};
} // namespace DHE
