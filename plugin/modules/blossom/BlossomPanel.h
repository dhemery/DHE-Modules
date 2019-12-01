#pragma once

#include <app/ModuleWidget.hpp>

namespace dhe {

class BlossomPanel : public rack::app::ModuleWidget {
public:
  explicit BlossomPanel(rack::engine::Module *blossom);
  static constexpr auto moduleSlug = "blossom";
};
} // namespace dhe
