#pragma once

#include "panel.h"
#include "modules/Xycloid.h"

namespace DHE {

class XycloidPanel : public Panel<XycloidPanel> {
public:
  explicit XycloidPanel(Xycloid *module);
  static constexpr auto module_slug = "xycloid";
  static constexpr auto hp = 11;
};
} // namespace DHE
