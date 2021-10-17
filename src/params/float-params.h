#pragma once
#include "rack.hpp"

namespace dhe {

template <typename T> struct LinearFloat {
  static inline void
  config(rack::engine::Module *module, int id, std::string const &name,
         float default_display_value = T::default_display_value) {
    auto const default_value =
        T::display_range.normalize(default_display_value);
    module->configParam(id, 0.F, 1.F, default_value, name, T::unit, 0.F,
                        T::display_range.size(),
                        T::display_range.lower_bound());
  }
};

struct Attenuverter : public LinearFloat<Attenuverter> {
  static auto constexpr display_range = Range{-100.F, 100.F};
  static auto constexpr default_display_value = 100.F;
  static auto constexpr unit = "%";
};

struct Gain : public LinearFloat<Gain> {
  static auto constexpr display_range = Range{0.F, 200.F};
  static auto constexpr default_display_value = 100.F;
  static auto constexpr unit = "%";
};

struct Percentage : public LinearFloat<Percentage> {
  static auto constexpr display_range = Range{0.F, 100.F};
  static auto constexpr default_display_value = 100.F;
  static auto constexpr unit = "%";
};

using Attenuator = Percentage;
} // namespace dhe
