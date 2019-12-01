#pragma once

#include <app/ModuleWidget.hpp>

namespace dhe {

class RangerPanel : public rack::app::ModuleWidget {
public:
  explicit RangerPanel(rack::engine::Module *module);
  static constexpr auto moduleSlug = "ranger";
};
} // namespace dhe
