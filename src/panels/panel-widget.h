#pragma once

#include "screws.h"

#include "rack.hpp"

namespace dhe {
template <typename TPanel> struct PanelWidget : public rack::app::ModuleWidget {
  PanelWidget(rack::engine::Module *module) {
    setModule(module);
    setPanel(load_svg(TPanel::svg_dir, TPanel::svg_name));
    install_screws(this, TPanel::hp);
  }
};

} // namespace dhe
