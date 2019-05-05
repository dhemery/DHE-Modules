#pragma once

#include "modules/Blossom.h"
#include "panel.h"

namespace DHE {

class BlossomPanel : public Panel<BlossomPanel> {
public:
  explicit BlossomPanel(Blossom *module);
  static constexpr auto module_slug = "blossom";
  static constexpr auto hp = 11;
};
} // namespace DHE
