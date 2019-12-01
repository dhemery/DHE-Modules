#pragma once

#include <app/ModuleWidget.hpp>
#include <engine/Module.hpp>

namespace dhe {

class TapersPanel : public rack::app::ModuleWidget {
public:
  explicit TapersPanel(rack::engine::Module *module);
  static constexpr auto moduleSlug = "tapers";
  static constexpr auto hp = 9;
};
} // namespace dhe
