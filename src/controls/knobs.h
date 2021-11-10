#pragma once

#include "components/meta.h"
#include "params/knob-quantity.h"
#include "params/mapped-knob-quantity.h"
#include "widgets/dimensions.h"
#include "widgets/knob-widget.h"

#include "rack.hpp"

#include <string>
#include <type_traits>

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
  template <typename, typename = void>
  struct has_knob_mapper : std::false_type {}; // NOLINT

  template <typename T>
  struct has_knob_mapper<T, void_t<typename T::KnobMapper>> // NOLINT
      : std::true_type {};

  template <typename TStyle, typename TPanel>
  static inline auto install(TPanel *panel, int id, float xmm, float ymm)
      -> KnobWidget<TPanel, TStyle, float> * {
    auto *widget = rack::createParamCentered<KnobWidget<TPanel, TStyle, float>>(
        mm2px(xmm, ymm), panel->getModule(), id);
    panel->addParam(widget);
    return widget;
  }

  // Use KnobQuantity if TConfig does not have a KnobMapper member type.
  template <typename TConfig, typename TValue = float>
  static inline auto config(rack::engine::Module *module, int id,
                            std::string const &name, TValue rotation = 0.5F)
      -> enable_if_t<!has_knob_mapper<TConfig>::value, KnobQuantity<float> *> {
    auto const multiplier = TConfig::display_range().size();
    auto const offset = TConfig::display_range().lower_bound();
    return module->configParam<KnobQuantity<float>>(
        id, 0.F, 1.F, rotation, name, TConfig::unit, 0.F, multiplier, offset);
  }

  // Use MappedKnobQuantity if TConfig has a KnobMapper member type.
  template <typename TConfig>
  static inline auto config(rack::engine::Module *module, int id,
                            std::string const &name, float rotation = 0.5F)
      -> enable_if_t<has_knob_mapper<TConfig>::value,
                     MappedKnobQuantity<TConfig> *> {
    return module->configParam<MappedKnobQuantity<TConfig>>(
        id, 0.F, 1.F, rotation, name, TConfig::unit);
  }
};

struct IntKnob {
  template <typename TStyle, typename TPanel>
  static auto install(TPanel *panel, int id, float xmm, float ymm)
      -> KnobWidget<TPanel, TStyle, int> * {
    auto *widget = rack::createParamCentered<KnobWidget<TPanel, TStyle, int>>(
        mm2px(xmm, ymm), panel->getModule(), id);
    panel->addParam(widget);
    widget->snap = true;
    return widget;
  }

  template <typename TConfig>
  static inline auto config(rack::engine::Module *module, int id,
                            std::string const &name, int value)
      -> KnobQuantity<int> * {
    auto const min = static_cast<float>(TConfig::min);
    auto const max = static_cast<float>(TConfig::max);
    auto const offset = static_cast<float>(TConfig::display_offset);
    auto const default_value = static_cast<float>(value);
    auto *pq = module->configParam<KnobQuantity<int>>(
        id, min, max, default_value, name, TConfig::unit, 0.F, 1.F, offset);
    pq->snapEnabled = true;
    return pq;
  }
};
} // namespace dhe
