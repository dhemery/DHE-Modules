#pragma once

#include "widgets/Panel.h"

namespace dhe {

class SwavePanel : public Panel<SwavePanel> {
public:
  explicit SwavePanel(rack::engine::Module *module);
  static constexpr auto moduleSlug = "swave";
  static constexpr auto hp = 4;
};

} // namespace dhe
