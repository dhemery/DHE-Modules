#pragma once

#include "panel.h"

namespace dhe {

namespace fuzzy_logic {
class HPanel : public Panel {
public:
  explicit HPanel(rack::engine::Module *module)
      : Panel{"fuzzy-logic-h", module} {}
};
} // namespace fuzzy_logic
} // namespace dhe
