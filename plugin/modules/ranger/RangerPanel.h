#pragma once

#include <app/ModuleWidget.hpp>
#include <engine/Module.hpp>

namespace dhe {

class RangerPanel : public rack::app::ModuleWidget {
public:
  explicit RangerPanel(rack::engine::Module *module);
  static constexpr auto moduleSlug = "ranger";
  static constexpr auto hp = 6;
};
} // namespace dhe
