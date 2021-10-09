#pragma once

#include "dimensions.h"
#include "panel-assets.h"

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

template <typename PanelT> struct Ports {
  static inline auto input(rack::engine::Module *module, float xmm, float ymm,
                           int index) -> PortWidget<PanelT> * {
    return rack::createInputCentered<PortWidget<PanelT>>(mm2px(xmm, ymm),
                                                         module, index);
  }

  static inline auto output(rack::engine::Module *module, float xmm, float ymm,
                            int index) -> PortWidget<PanelT> * {
    return rack::createOutputCentered<PortWidget<PanelT>>(mm2px(xmm, ymm),
                                                          module, index);
  }
};

} // namespace dhe
