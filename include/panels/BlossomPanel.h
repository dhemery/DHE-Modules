#pragma once

#include "Panel.h"
#include "modules/Blossom.h"

namespace dhe {

class BlossomPanel : public Panel<BlossomPanel> {
public:
  explicit BlossomPanel(Blossom *blossom);
  static constexpr auto moduleSlug = "blossom";
  static constexpr auto hp = 11;
};
} // namespace dhe
