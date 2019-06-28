#pragma once

#include "Panel.h"
#include "modules/FuzzyZ.h"

namespace dhe {

class FuzzyZPanel : public Panel<FuzzyZPanel> {
public:
  explicit FuzzyZPanel(FuzzyZ *fuzzy);
  static constexpr auto moduleSlug = "fuzzy-z";
  static constexpr auto hp = 9;
};
} // namespace dhe
