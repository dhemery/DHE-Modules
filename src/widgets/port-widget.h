#pragma once

#include "assets.h"

#include "rack.hpp"

namespace dhe {

template <typename PanelT> struct PortWidget : rack::app::SvgPort {
  PortWidget() {
    setSvg(load_svg(PanelT::svg_dir, "port"));
    shadow->opacity = 0.F;
  }
};
} // namespace dhe
