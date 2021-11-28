#pragma once

#include "panels/assets.h"
#include "panels/dimensions.h"

#include "rack.hpp"

#include <string>

namespace dhe {

template <typename PanelT> struct PortWidget : rack::app::SvgPort {
  PortWidget() {
    setSvg(load_svg(PanelT::svg_dir, "port"));
    shadow->opacity = 0.F;
  }
};

struct InPort {
  template <typename P>
  static inline void install(P *panel, int id, float xmm, float ymm) {
    panel->addInput(rack::createInputCentered<PortWidget<P>>(
        mm2px(xmm, ymm), panel->getModule(), id));
  }

  template <typename M>
  static inline void config(M *module, int id, std::string const &name) {
    module->configInput(id, name);
  }
};

struct OutPort {
  template <typename P>
  static inline void install(P *panel, int id, float xmm, float ymm) {
    panel->addOutput(rack::createOutputCentered<PortWidget<P>>(
        mm2px(xmm, ymm), panel->getModule(), id));
  }

  template <typename M>
  static inline void config(M *module, int id, std::string const &name) {
    module->configOutput(id, name);
  }
};

} // namespace dhe
