#pragma once

#include "Panel.h"
#include "modules/Xycloid.h"

namespace dhe {

class XycloidPanel : public Panel<XycloidPanel> {
public:
  explicit XycloidPanel(Xycloid *module);
  static constexpr auto moduleSlug = "xycloid";
  static constexpr auto hp = 11;
};
} // namespace dhe
