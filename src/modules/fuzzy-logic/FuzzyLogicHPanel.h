#pragma once

#include "FuzzyLogicPanel.h"

namespace dhe {

namespace fuzzy_logic {
class FuzzyLogicHPanel : public FuzzyLogicPanel {
public:
  explicit FuzzyLogicHPanel(rack::engine::Module *module)
      : FuzzyLogicPanel{"fuzzy-logic-h", module} {}
};
} // namespace fuzzy_logic
} // namespace dhe
