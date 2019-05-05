#pragma once

#include "modules/Stage.h"
#include "panel.h"

namespace DHE {
class StagePanel : public Panel<StagePanel> {
public:
  explicit StagePanel(Stage *module);
  static constexpr auto module_slug = "stage";
  static constexpr auto hp = 5;
};
} // namespace DHE
