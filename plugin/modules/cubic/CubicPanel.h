#pragma once

#include <app/ModuleWidget.hpp>

namespace dhe {

class CubicPanel : public rack::app::ModuleWidget {
public:
  explicit CubicPanel(rack::engine::Module *module);
};
} // namespace dhe
