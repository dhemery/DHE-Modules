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
  struct is_mapped : std::false_type {}; // NOLINT

  // Determines whether T has a DisplayMapper member type.
  template <typename T>
  struct is_mapped<T, void_t<typename T::DisplayMapper>> // NOLINT
      : std::true_type {};

  template <typename TStyle, typename TPanel>
  static inline auto install(TPanel *panel, int id, float xmm, float ymm)
      -> KnobWidget<TPanel, TStyle, float> * {
    auto *widget = rack::createParamCentered<KnobWidget<TPanel, TStyle, float>>(
        mm2px(xmm, ymm), panel->getModule(), id);
    panel->addParam(widget);
    return widget;
  }

  // Enabled if TRange has no DisplayMapper member type.
  template <typename TRange>
  static inline auto config(rack::engine::Module *module, int id,
                            std::string const &name, float rotation = 0.5F)
      -> enable_if_t<!is_mapped<TRange>::value, KnobQuantity<float> *> {
    auto const multiplier = TRange::display_range().size();
    auto const offset = TRange::display_range().lower_bound();
    return module->configParam<KnobQuantity<float>>(
        id, 0.F, 1.F, rotation, name, TRange::unit, 0.F, multiplier, offset);
  }

  // Enabled if TMapped has a DisplayMapper member type.
  template <typename TMapped>
  static inline auto config(rack::engine::Module *module, int id,
                            std::string const &name, float rotation = 0.5F)
      -> enable_if_t<is_mapped<TMapped>::value, MappedKnobQuantity<TMapped> *> {
    return module->configParam<MappedKnobQuantity<TMapped>>(
        id, 0.F, 1.F, rotation, name, TMapped::unit);
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

  template <typename TInts>
  static inline auto config(rack::engine::Module *module, int id,
                            std::string const &name, int value)
      -> KnobQuantity<int> * {
    auto const min = static_cast<float>(TInts::min);
    auto const max = static_cast<float>(TInts::max);
    auto const offset = static_cast<float>(TInts::display_offset);
    auto const default_value = static_cast<float>(value);
    auto *pq = module->configParam<KnobQuantity<int>>(
        id, min, max, default_value, name, TInts::unit, 0.F, 1.F, offset);
    pq->snapEnabled = true;
    return pq;
  }
};
} // namespace dhe
