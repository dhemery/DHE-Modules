#pragma once

#include "modules/Upstage.h"
#include "panel.h"

namespace DHE {

class UpstagePanel : public Panel<UpstagePanel> {
public:
  explicit UpstagePanel(Upstage *module);
  static constexpr auto module_slug = "upstage";
  static constexpr auto hp = 5;
};

} // namespace DHE
