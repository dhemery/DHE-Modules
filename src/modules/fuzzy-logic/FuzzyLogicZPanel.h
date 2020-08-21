#pragma once

#include "FuzzyLogicPanel.h"

namespace dhe {

namespace fuzzy_logic {
class FuzzyLogicZPanel : public FuzzyLogicPanel {
public:
  explicit FuzzyLogicZPanel(rack::engine::Module *module)
      : FuzzyLogicPanel{"fuzzy-logic-z", module} {}
};
} // namespace fuzzy_logic
} // namespace dhe
