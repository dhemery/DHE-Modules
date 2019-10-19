#pragma once

#include "FuzzyLogicH.h"
#include "modules/Panel.h"

namespace dhe {

class FuzzyLogicHPanel : public Panel<FuzzyLogicHPanel> {
public:
  explicit FuzzyLogicHPanel(FuzzyLogicH *fuzzy);
  static constexpr auto moduleSlug = "fuzzy-logic-h";
  static constexpr auto hp = 9;
};
} // namespace dhe
