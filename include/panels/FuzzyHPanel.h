#pragma once

#include "Panel.h"
#include "modules/FuzzyH.h"

namespace dhe {

class FuzzyHPanel : public Panel<FuzzyHPanel> {
public:
  explicit FuzzyHPanel(FuzzyH *fuzzy);
  static constexpr auto moduleSlug = "fuzzy-h";
  static constexpr auto hp = 9;
};
} // namespace dhe
