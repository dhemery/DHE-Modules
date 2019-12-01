#pragma once

#include <app/ModuleWidget.hpp>

namespace dhe {

class FuzzyLogicHPanel : public rack::app::ModuleWidget {
public:
  explicit FuzzyLogicHPanel(rack::engine::Module *fuzzy);
  static constexpr auto moduleSlug = "fuzzy-logic-h";
};
} // namespace dhe
