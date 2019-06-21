#pragma once

#include "Panel.h"
#include "modules/Gator.h"

namespace dhe {

class GatorPanel : public Panel<GatorPanel> {
public:
  explicit GatorPanel(Gator *gator);
  static constexpr auto moduleSlug = "gator";
  static constexpr auto hp = 9;
};
} // namespace dhe
