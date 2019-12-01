#pragma once

#include <app/ModuleWidget.hpp>

namespace dhe {

class FuzzyLogicHPanel : public rack::app::ModuleWidget {
public:
  explicit FuzzyLogicHPanel(rack::engine::Module *fuzzy);
  static constexpr auto moduleSlug = "fuzzy-logic-h";
  static constexpr auto hp = 9;
};
} // namespace dhe
