#pragma once

#include "widgets/Panel.h"

namespace dhe {

class FuzzyLogicZPanel : public Panel<FuzzyLogicZPanel> {
public:
  explicit FuzzyLogicZPanel(rack::engine::Module *module);
  static constexpr auto moduleSlug = "fuzzy-logic-z";
  static constexpr auto hp = 9;
};
} // namespace dhe
