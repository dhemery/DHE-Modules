#pragma once

#include "panel.h"

namespace dhe {

namespace fuzzy_logic {
class ZPanel : public Panel {
public:
  explicit ZPanel(rack::engine::Module *module)
      : Panel{"fuzzy-logic-z", module} {}
};
} // namespace fuzzy_logic
} // namespace dhe
