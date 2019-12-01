#pragma once

#include <app/ModuleWidget.hpp>

namespace dhe {

class XycloidPanel : public rack::app::ModuleWidget {
public:
  explicit XycloidPanel(rack::engine::Module *module);
  static constexpr auto moduleSlug = "xycloid";
};
} // namespace dhe
