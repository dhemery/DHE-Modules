#pragma once

#include "Upstage.h"
#include "widgets/Panel.h"

namespace dhe {

class UpstagePanel : public Panel<UpstagePanel> {
public:
  explicit UpstagePanel(Upstage *module);
  static constexpr auto moduleSlug = "upstage";
  static constexpr auto hp = 5;
};

} // namespace dhe
