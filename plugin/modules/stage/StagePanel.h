#pragma once

#include "Stage.h"
#include "modules/Panel.h"

namespace dhe {
class StagePanel : public Panel<StagePanel> {
public:
  explicit StagePanel(Stage *module);
  static constexpr auto moduleSlug = "stage";
  static constexpr auto hp = 5;
};
} // namespace dhe
