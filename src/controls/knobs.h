#pragma once

#include "components/meta.h"
#include "params/mapped-knob-quantity.h"
#include "params/ranged-knob-quantity.h"
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
  struct defines_display_range : std::false_type {};

  template <typename R>
  struct defines_display_range<
      R, void_t<decltype(R::display_min), decltype(R::display_max)>>
      : std::is_floating_point<decltype(R::display_min)> {};

  template <typename, typename = void>
  struct defines_int_range : std::false_type {};

  template <typename R>
  struct defines_int_range<R, void_t<decltype(R::min), decltype(R::max)>>
      : std::is_integral<decltype(R::min)> {};

  template <typename, typename = void> struct is_mapped : std::false_type {};

  template <typename M>
  struct is_mapped<M, void_t<typename M::KnobMapper>> : std::true_type {};

  template <typename TStyle, typename TValue = float, typename TPanel>
  static inline auto install(TPanel *panel, int id, float xmm, float ymm)
      -> KnobWidget<TPanel, TStyle, TValue> * {
    auto *widget =
        rack::createParamCentered<KnobWidget<TPanel, TStyle, TValue>>(
            mm2px(xmm, ymm), panel->getModule(), id);
    widget->snap = std::is_integral<TValue>::value;
    panel->addParam(widget);
    return widget;
  }

  // Configure a knob with the display range specified by F
  template <typename F>
  static inline auto config(rack::engine::Module *module, int id,
                            std::string const &name,
                            float value = F::display_default)
      -> enable_if_t<defines_display_range<F>::value,
                     RangedKnobQuantity<float> *> {
    auto const display_range = F::display_max - F::display_min;
    auto const rotation = cx::normalize(value, F::display_min, F::display_max);
    return module->configParam<RangedKnobQuantity<float>>(
        id, 0.F, 1.F, rotation, name, F::unit, 0.F, display_range,
        F::display_min);
  }

  // Configure a knob with the int range specified by I
  template <typename I>
  static inline auto config(rack::engine::Module *module, int id,
                            std::string const &name, int value)
      -> enable_if_t<defines_int_range<I>::value, RangedKnobQuantity<int> *> {
    auto const min = static_cast<float>(I::min);
    auto const max = static_cast<float>(I::max);
    auto const default_value = static_cast<float>(value);
    auto const rotation = cx::normalize(default_value, min, max);
    auto *q = module->configParam<RangedKnobQuantity<int>>(
        id, min, max, rotation, name, I::unit);
    q->snapEnabled = true;
    return q;
  }

  // Configure a knob with display values mapped by M
  template <typename M>
  static inline auto config(rack::engine::Module *module, int id,
                            std::string const &name,
                            float rotation = M::default_rotation)
      -> enable_if_t<is_mapped<M>::value, MappedKnobQuantity<M> *> {
    return module->configParam<MappedKnobQuantity<M>>(id, 0.F, 1.F, rotation,
                                                      name, M::unit);
  }
};
} // namespace dhe
