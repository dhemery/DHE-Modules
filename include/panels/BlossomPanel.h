#pragma once

#include "Panel.h"
#include "modules/Blossom.h"

namespace DHE {

class BlossomPanel : public Panel<BlossomPanel> {
public:
  explicit BlossomPanel(Blossom *blossom);
  static constexpr auto module_slug = "blossom";
  static constexpr auto hp = 11;
};
} // namespace DHE
