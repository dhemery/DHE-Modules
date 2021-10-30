#pragma once

#include "components/range.h"
#include "panels/dimensions.h"
#include "panels/panel-assets.h"

#include "rack.hpp"

#include <string>
#include <vector>

namespace dhe {

struct Knob {
  template <typename TPanel, typename TStyle>
  struct Widget : public rack::app::SvgKnob {
    Widget() {
      auto knob_svg = load_svg(TPanel::svg_dir, TStyle::svg_file);
      setSvg(knob_svg);
      minAngle = -0.83F * pi;
      maxAngle = 0.83F * M_PI;
      shadow->opacity = 0.F;
    }
  };

  template <typename TStyle, typename TPanel>
  static inline void install(TPanel *panel, int id, float xmm, float ymm) {
    panel->addParam(rack::createParamCentered<Widget<TPanel, TStyle>>(
        mm2px(xmm, ymm), panel->getModule(), id));
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

template <typename TQuantity> struct LinearKnob {
  static inline void config(rack::engine::Module *module, int id,
                            std::string const &name,
                            float initial_value = TQuantity::initial) {
    static auto const range = TQuantity::range();
    auto const default_value = range.normalize(initial_value);
    module->configParam(id, 0.F, 1.F, default_value, name, TQuantity::unit, 0.F,
                        range.size(), range.lower_bound());
  }
};

template <typename TQuantity> struct ScaledKnob {
  static inline void config(rack::engine::Module *module, int id,
                            std::string const &name,
                            float initial_value = TQuantity::initial) {
    static auto const range = TQuantity::range();
    module->configParam(id, range.lower_bound(), range.upper_bound(),
                        initial_value, name, TQuantity::unit);
  }
};

struct Attenuverter : public LinearKnob<Attenuverter> {
  static auto constexpr range() -> Range { return Range{-100.F, 100.F}; }
  static auto constexpr initial = 100.F;
  static auto constexpr unit = "%";
};

struct Gain : public LinearKnob<Gain> {
  static auto constexpr range() -> Range { return Range{0.F, 200.F}; }
  static auto constexpr initial = 100.F;
  static auto constexpr unit = "%";
};

struct Percentage : public LinearKnob<Percentage> {
  static auto constexpr range() -> Range { return Range{0.F, 100.F}; }
  static auto constexpr initial = 100.F;
  static auto constexpr unit = "%";
};

using Attenuator = Percentage;
} // namespace dhe
