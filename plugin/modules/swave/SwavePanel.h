#pragma once

#include <app/ModuleWidget.hpp>

namespace dhe {

class SwavePanel : public rack::app::ModuleWidget {
public:
  explicit SwavePanel(rack::engine::Module *module);
};

} // namespace dhe
