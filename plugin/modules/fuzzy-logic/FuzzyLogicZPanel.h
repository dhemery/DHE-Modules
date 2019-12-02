#pragma once

#include <app/ModuleWidget.hpp>

namespace dhe {

class FuzzyLogicZPanel : public rack::app::ModuleWidget {
public:
  explicit FuzzyLogicZPanel(rack::engine::Module *module);
};
} // namespace dhe
