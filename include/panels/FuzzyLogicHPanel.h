#pragma once

#include "Panel.h"
#include "modules/FuzzyLogicH.h"

namespace dhe {

class FuzzyLogicHPanel : public Panel<FuzzyLogicHPanel> {
public:
  explicit FuzzyLogicHPanel(FuzzyLogicH *fuzzy);
  static constexpr auto moduleSlug = "fuzzy-logic-h";
  static constexpr auto hp = 9;
};
} // namespace dhe
