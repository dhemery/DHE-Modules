#pragma once

#include "Tapers.h"
#include "modules/Panel.h"

namespace dhe {

class TapersPanel : public Panel<TapersPanel> {
public:
  explicit TapersPanel(Tapers *module);
  static constexpr auto moduleSlug = "tapers";
  static constexpr auto hp = 9;
};
} // namespace dhe
