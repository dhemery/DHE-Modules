#pragma once

#include "widgets/dimensions.h"
#include "widgets/panel-assets.h"

#include <app/SvgPort.hpp>
#include <helpers.hpp>

#include <string>

namespace dhe {
template <typename PanelT> struct PortWidget : rack::app::SvgPort {
  PortWidget() {
    setSvg(load_svg(PanelT::svg_dir, "port"));
    shadow->opacity = 0.F;
  }
};

struct Input {
  template <typename PanelT>
  static inline void install(PanelT *panel, int id, float xmm, float ymm) {
    panel->addInput(rack::createInputCentered<PortWidget<PanelT>>(
        mm2px(xmm, ymm), panel->getModule(), id));
  }

  template <typename ModuleT>
  static inline void config(ModuleT *module, int id, std::string const &name) {
    module->configInput(id, name);
  }
};

struct Output {
  template <typename PanelT>
  static inline void install(PanelT *panel, int id, float xmm, float ymm) {
    panel->addOutput(rack::createOutputCentered<PortWidget<PanelT>>(
        mm2px(xmm, ymm), panel->getModule(), id));
  }

  template <typename ModuleT>
  static inline void config(ModuleT *module, int id, std::string const &name) {
    module->configOutput(id, name);
  }
};
} // namespace dhe
