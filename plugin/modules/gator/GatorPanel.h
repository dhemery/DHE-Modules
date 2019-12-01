#pragma once

#include <app/ModuleWidget.hpp>

namespace dhe {

class GatorPanel : public rack::app::ModuleWidget {
public:
  explicit GatorPanel(rack::engine::Module *module);
  static constexpr auto moduleSlug = "gator";
  static constexpr auto hp = 9;
};
} // namespace dhe
