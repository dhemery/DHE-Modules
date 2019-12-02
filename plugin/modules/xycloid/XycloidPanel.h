#pragma once

#include <app/ModuleWidget.hpp>

namespace dhe {

class XycloidPanel : public rack::app::ModuleWidget {
public:
  explicit XycloidPanel(rack::engine::Module *module);
};
} // namespace dhe
