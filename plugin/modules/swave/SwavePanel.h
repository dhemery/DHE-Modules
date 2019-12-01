#pragma once

#include <app/ModuleWidget.hpp>
#include <engine/Module.hpp>

namespace dhe {

class SwavePanel : public rack::app::ModuleWidget {
public:
  explicit SwavePanel(rack::engine::Module *module);
  static constexpr auto moduleSlug = "swave";
  static constexpr auto hp = 4;
};

} // namespace dhe
