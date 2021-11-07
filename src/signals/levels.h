#pragma once

#include "components/range.h"

#include <string>
#include <vector>

namespace dhe {

struct Unipolar {
  static auto constexpr range = Range{0.F, 10.F};
  static auto constexpr display_range = range;
  static auto constexpr label = "0–10 V";
  static auto constexpr unit = " V";
};

struct Bipolar {
  static auto constexpr range = Range{-5.F, 5.F};
  static auto constexpr display_range = range;
  static auto constexpr label = "±5 V";
  static auto constexpr unit = " V";
};

struct Levels {
  using ItemType = Range;

  enum { Bipolar, Unipolar };

  static auto constexpr unit = " V";

  static inline auto items() -> std::vector<Range> const & {
    static auto const items =
        std::vector<Range>{Bipolar::range, Unipolar::range};
    return items;
  }

  static inline auto labels() -> std::vector<std::string> const & {
    static auto const labels =
        std::vector<std::string>{Bipolar::label, Unipolar::label};
    return labels;
  }
};

} // namespace dhe
