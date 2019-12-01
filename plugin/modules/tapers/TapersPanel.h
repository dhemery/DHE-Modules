#pragma once

#include <app/ModuleWidget.hpp>

namespace dhe {

class TapersPanel : public rack::app::ModuleWidget {
public:
  explicit TapersPanel(rack::engine::Module *module);
  static constexpr auto moduleSlug = "tapers";
};
} // namespace dhe
