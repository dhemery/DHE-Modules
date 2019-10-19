#pragma once

#include "FuzzyLogicZ.h"
#include "modules/Panel.h"

namespace dhe {

class FuzzyLogicZPanel : public Panel<FuzzyLogicZPanel> {
public:
  explicit FuzzyLogicZPanel(FuzzyLogicZ *fuzzy);
  static constexpr auto moduleSlug = "fuzzy-logic-z";
  static constexpr auto hp = 9;
};
} // namespace dhe
