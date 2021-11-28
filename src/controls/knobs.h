#pragma once

#include "components/meta.h"
#include "params/mapped-knob-quantity.h"
#include "params/ranged-knob-quantity.h"
#include "signals/linear-signals.h"
#include "widgets/assets.h"
#include "widgets/dimensions.h"

#include "rack.hpp"

#include <string>

namespace dhe {

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
  template <typename Panel, typename Style> struct Widget : rack::app::SvgKnob {
    Widget() {
      auto knob_svg = load_svg(Panel::svg_dir, Style::svg_file);
      setSvg(knob_svg);
      minAngle = -0.83F * pi;
      maxAngle = 0.83F * M_PI;
      shadow->opacity = 0.F;
    }
  };

  template <typename Style, typename Panel>
  static inline auto install(Panel *panel, int id, float xmm, float ymm)
      -> Widget<Panel, Style> * {
    auto *widget = rack::createParamCentered<Widget<Panel, Style>>(
        mm2px(xmm, ymm), panel->getModule(), id);
    panel->addParam(widget);
    return widget;
  }
};

template <typename Scale> struct LinearKnob {
  static inline auto config(rack::engine::Module *module, int id,
                            std::string const &name,
                            float default_value = Scale::default_value)
      -> rack::engine::ParamQuantity * {
    auto const default_rotation = Scale::range.normalize(default_value);
    auto *q = module->configParam(id, 0.F, 1.F, default_rotation, name,
                                  Scale::unit, 0.F, Scale::range.size(),
                                  Scale::range.lower_bound());
    return q;
  }
};

namespace linear {
static auto constexpr attenuverter_knob_range = Range{-100.F, 100.F};
static auto constexpr gain_knob_range = Range{0.F, 200.F};
static auto constexpr percentage_knob_range = Range{0.F, 100.F};
static auto constexpr phase_knob_range = Range{-180.F, 180.F};
} // namespace linear

struct AttenuverterKnob : LinearKnob<AttenuverterKnob> {
  static auto constexpr default_value = 0.F;
  static auto constexpr &range = linear::attenuverter_knob_range;
  static auto constexpr unit = "%";
};

struct GainKnob : LinearKnob<GainKnob> {
  static auto constexpr default_value = 100.F;
  static auto constexpr &range = linear::gain_knob_range;
  static auto constexpr unit = "%";
};

struct PercentageKnob : LinearKnob<PercentageKnob> {
  static auto constexpr default_value = 100.F;
  static auto constexpr &range = linear::percentage_knob_range;
  static auto constexpr unit = "%";
};

struct PhaseKnob : LinearKnob<PhaseKnob> {
  static auto constexpr default_value = 0.F;
  static auto constexpr &range = linear::phase_knob_range;
  static auto constexpr unit = "˚";
};

} // namespace dhe
