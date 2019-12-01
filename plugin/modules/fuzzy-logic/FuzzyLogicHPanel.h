#pragma once

#include "widgets/Panel.h"

namespace dhe {

class FuzzyLogicHPanel : public Panel<FuzzyLogicHPanel> {
public:
  explicit FuzzyLogicHPanel(rack::engine::Module *fuzzy);
  static constexpr auto moduleSlug = "fuzzy-logic-h";
  static constexpr auto hp = 9;
};
} // namespace dhe
