#pragma once

#include "display/panel.h"

namespace DHE {

class BoosterStagePanel : public Panel<BoosterStagePanel> {
public:
  BoosterStagePanel(BoosterStage *module);
  static constexpr auto module_slug = "booster-stage";
  static constexpr auto hp = 8;
};
}
