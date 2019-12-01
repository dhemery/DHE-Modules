#pragma once

#include <app/ModuleWidget.hpp>

namespace dhe {

class FuzzyLogicZPanel : public rack::app::ModuleWidget {
public:
  explicit FuzzyLogicZPanel(rack::engine::Module *module);
  static constexpr auto moduleSlug = "fuzzy-logic-z";
  static constexpr auto hp = 9;
};
} // namespace dhe
