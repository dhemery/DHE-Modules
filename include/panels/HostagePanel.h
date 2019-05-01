#pragma once

#include "display/panel.h"
#include "modules/Hostage.h"

namespace DHE {

class HostagePanel : public Panel<HostagePanel> {
public:
  explicit HostagePanel(Hostage *module);
  static constexpr auto module_slug = "hostage";
  static constexpr auto hp = 5;
};
} // namespace DHE
