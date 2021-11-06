#pragma once

#include "components/range.h"
#include "controls/range-switch-quantity.h"
#include "controls/scaled-param-quantity.h"

#include "rack.hpp"

#include <functional>
#include <string>
#include <vector>

namespace dhe {

struct Levels {
  enum { Bipolar, Unipolar };

  static auto constexpr bipolar_range = Range{-5.F, 5.F};
  static auto constexpr unipolar_range = Range{0.F, 10.F};

  static auto constexpr unit = " V";

  static inline auto ranges() -> std::vector<Range> const & {
    static auto const ranges =
        std::vector<Range>{bipolar_range, unipolar_range};
    return ranges;
  }

  static inline auto labels() -> std::vector<std::string> const & {
    static auto const labels = std::vector<std::string>{"±5 V", "0–10 V"};
    return labels;
  }
};

struct LevelSwitch {
  using TQuantity = RangeSwitchQuantity<Levels>;

  static inline auto config(rack::engine::Module *module, int id,
                            std::string const &name,
                            int selection = Levels::Unipolar) -> TQuantity * {
    auto const max_value = static_cast<float>(Levels::ranges().size() - 1);
    auto const default_value = static_cast<float>(selection);
    auto *pq = module->configSwitch<TQuantity>(
        id, 0.F, max_value, default_value, name, Levels::labels());
    return pq;
  }
};

struct LevelKnob {
  static inline auto config(rack::engine::Module *module, int id,
                            std::string const &name, float initial_value = 5.F,
                            int initial_range = Levels::Unipolar)
      -> ScaledParamQuantity * {
    auto const range = Levels::ranges()[initial_range];
    auto *pq = module->configParam<ScaledParamQuantity>(
        id, range.lower_bound(), range.upper_bound(), initial_value, name,
        Levels::unit);
    return pq;
  }
};
} // namespace dhe
