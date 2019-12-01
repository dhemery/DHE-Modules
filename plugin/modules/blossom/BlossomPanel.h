#pragma once

#include <app/ModuleWidget.hpp>
#include <engine/Module.hpp>

namespace dhe {

class BlossomPanel : public rack::app::ModuleWidget {
public:
  explicit BlossomPanel(rack::engine::Module *blossom);
  static constexpr auto moduleSlug = "blossom";
  static constexpr auto hp = 11;
};
} // namespace dhe
