#pragma once

#include <app/ModuleWidget.hpp>

namespace dhe {

class SwavePanel : public rack::app::ModuleWidget {
public:
  explicit SwavePanel(rack::engine::Module *module);
  static constexpr auto moduleSlug = "swave";
};

} // namespace dhe
