#pragma once

#include "widgets/Panel.h"

namespace dhe {

class XycloidPanel : public Panel<XycloidPanel> {
public:
  explicit XycloidPanel(rack::engine::Module *module);
  static constexpr auto moduleSlug = "xycloid";
  static constexpr auto hp = 11;
};
} // namespace dhe
