#pragma once

#include "widgets/Panel.h"

namespace dhe {

class RangerPanel : public Panel<RangerPanel> {
public:
  explicit RangerPanel(rack::engine::Module *module);
  static constexpr auto moduleSlug = "ranger";
  static constexpr auto hp = 6;
};
} // namespace dhe
