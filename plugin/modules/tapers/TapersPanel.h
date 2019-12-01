#pragma once

#include "widgets/Panel.h"

namespace dhe {

class TapersPanel : public Panel<TapersPanel> {
public:
  explicit TapersPanel(rack::engine::Module *module);
  static constexpr auto moduleSlug = "tapers";
  static constexpr auto hp = 9;
};
} // namespace dhe
