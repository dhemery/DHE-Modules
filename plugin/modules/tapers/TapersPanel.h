#pragma once

#include <app/ModuleWidget.hpp>

namespace dhe {

class TapersPanel : public rack::app::ModuleWidget {
public:
  explicit TapersPanel(rack::engine::Module *module);
};
} // namespace dhe
