#pragma once

#include "./dimensions.h"
#include "./panel-assets.h"
#include "components/cxmath.h"

#include "rack.hpp"

#include <string>

namespace dhe {
template <typename PanelT, typename KnobT>
struct KnobWidget : public rack::app::SvgKnob {
  KnobWidget() {
    auto knob_svg = load_svg(PanelT::svg_dir, KnobT::svg_file);
    setSvg(knob_svg);
    minAngle = -0.83F * pi;
    maxAngle = 0.83F * M_PI;
    shadow->opacity = 0.F;
  }
};

struct Large {
  static auto constexpr svg_file = "knob-large";
};

struct Medium {
  static auto constexpr svg_file = "knob-medium";
};

struct Small {
  static auto constexpr svg_file = "knob-small";
};

struct Tiny {
  static auto constexpr svg_file = "knob-tiny";
};

struct Knob {
  template <typename SizeT, typename PanelT>
  static inline void install(PanelT *panel, int id, float xmm, float ymm) {
    panel->addParam(rack::createParamCentered<KnobWidget<PanelT, SizeT>>(
        mm2px(xmm, ymm), panel->getModule(), id));
  }
};
} // namespace dhe
