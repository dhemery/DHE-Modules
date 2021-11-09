#pragma once

#include "params/curvature-knob-quantity.h"
#include "params/duration-knob-quantity.h"
#include "params/knob-quantity.h"
#include "widgets/dimensions.h"
#include "widgets/knob-widget.h"

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
  template <typename TStyle, typename TPanel>
  static inline auto install(TPanel *panel, int id, float xmm, float ymm)
      -> KnobWidget<TPanel, TStyle, float> * {
    auto *widget = rack::createParamCentered<KnobWidget<TPanel, TStyle, float>>(
        mm2px(xmm, ymm), panel->getModule(), id);
    panel->addParam(widget);
    return widget;
  }

  template <typename TQuantity>
  static inline auto config(rack::engine::Module *module, int id,
                            std::string const &name, float rotation = 0.5F)
      -> KnobQuantity<float> * {
    auto const multiplier = TQuantity::display_range().size();
    auto const offset = TQuantity::display_range().lower_bound();
    return module->configParam<KnobQuantity<float>>(
        id, 0.F, 1.F, rotation, name, TQuantity::unit, 0.F, multiplier, offset);
  }
};

struct IntKnob {
  template <typename TStyle, typename TPanel>
  static inline auto install(TPanel *panel, int id, float xmm, float ymm)
      -> KnobWidget<TPanel, TStyle, int> * {
    auto *widget = rack::createParamCentered<KnobWidget<TPanel, TStyle, int>>(
        mm2px(xmm, ymm), panel->getModule(), id);
    panel->addParam(widget);
    widget->snap = true;
    return widget;
  }

  template <typename TQuantity>
  static inline auto config(rack::engine::Module *module, int id,
                            std::string const &name, int value)
      -> KnobQuantity<int> * {
    auto const min = static_cast<float>(TQuantity::min);
    auto const max = static_cast<float>(TQuantity::max);
    auto const offset = static_cast<float>(TQuantity::display_offset);
    auto const default_value = static_cast<float>(value);
    auto *pq = module->configParam<KnobQuantity<int>>(
        id, min, max, default_value, name, TQuantity::unit, 0.F, 1.F, offset);
    pq->snapEnabled = true;
    return pq;
  }
};

struct DurationKnob {
  template <typename TStyle, typename TPanel>
  static inline auto install(TPanel *panel, int id, float xmm, float ymm)
      -> KnobWidget<TPanel, TStyle> * {
    auto *widget = rack::createParamCentered<KnobWidget<TPanel, TStyle>>(
        mm2px(xmm, ymm), panel->getModule(), id);
    panel->addParam(widget);
    return widget;
  }

  static inline auto config(rack::engine::Module *module, int id,
                            std::string const &name, float rotation = 0.5F)
      -> DurationKnobQuantity * {
    return module->configParam<DurationKnobQuantity>(id, 0.F, 1.F, rotation,
                                                     name, Durations::unit);
  }
};

struct CurvatureKnob {
  template <typename TStyle, typename TPanel>
  static inline auto install(TPanel *panel, int id, float xmm, float ymm)
      -> KnobWidget<TPanel, TStyle> * {
    auto *widget = rack::createParamCentered<KnobWidget<TPanel, TStyle>>(
        mm2px(xmm, ymm), panel->getModule(), id);
    panel->addParam(widget);
    return widget;
  }

  static inline auto config(rack::engine::Module *module, int id,
                            std::string const &name, float rotation = 0.5F)
      -> CurvatureKnobQuantity * {
    return module->configParam<CurvatureKnobQuantity>(id, 0.F, 1.F, rotation,
                                                      name, Durations::unit);
  }
};

} // namespace dhe
