#pragma once

#include "widgets/Panel.h"

namespace dhe {

class GatorPanel : public Panel<GatorPanel> {
public:
  explicit GatorPanel(rack::engine::Module *module);
  static constexpr auto moduleSlug = "gator";
  static constexpr auto hp = 9;
};
} // namespace dhe
