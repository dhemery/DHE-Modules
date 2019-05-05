#pragma once

#include "panel.h"
#include "modules/Blossom.h"

namespace DHE {

class BlossomPanel : public Panel<BlossomPanel> {
public:
  explicit BlossomPanel(Blossom *module);
  static constexpr auto module_slug = "blossom";
  static constexpr auto hp = 11;
};
} // namespace DHE
