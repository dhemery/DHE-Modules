#pragma once

#include "panel.h"

namespace dhe {

namespace fuzzy_logic {
class FuzzyLogicZPanel : public Panel {
public:
  explicit FuzzyLogicZPanel(rack::engine::Module *module)
      : Panel{"fuzzy-logic-z", module} {}
};
} // namespace fuzzy_logic
} // namespace dhe
