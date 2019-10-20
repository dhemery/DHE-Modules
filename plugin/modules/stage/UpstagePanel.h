#pragma once

#include "Panel.h"
#include "Upstage.h"

namespace dhe {

class UpstagePanel : public Panel<UpstagePanel> {
public:
  explicit UpstagePanel(Upstage *module);
  static constexpr auto moduleSlug = "upstage";
  static constexpr auto hp = 5;
};

} // namespace dhe
