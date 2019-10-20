#pragma once

#include "Panel.h"
#include "Ranger.h"

namespace dhe {

class RangerPanel : public Panel<RangerPanel> {
public:
  explicit RangerPanel(Ranger *module);
  static constexpr auto moduleSlug = "ranger";
  static constexpr auto hp = 6;
};
} // namespace dhe
