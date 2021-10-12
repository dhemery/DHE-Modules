#pragma once

#include "./panel.h"

namespace dhe {

namespace fuzzy_logic {
class FuzzyLogicHPanel : public Panel {
public:
  explicit FuzzyLogicHPanel(rack::engine::Module *module)
      : Panel{"fuzzy-logic-h", module} {}
};
} // namespace fuzzy_logic
} // namespace dhe
