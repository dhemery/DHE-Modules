#pragma once

#include "modules/Hostage.h"
#include "panel.h"

namespace DHE {

class HostagePanel : public Panel<HostagePanel> {
public:
  explicit HostagePanel(Hostage *module);
  static constexpr auto module_slug = "hostage";
  static constexpr auto hp = 5;
};
} // namespace DHE
