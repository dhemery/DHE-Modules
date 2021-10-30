#pragma once

#include "components/range.h"

#include <string>
#include <vector>

namespace dhe {
namespace envelope {
namespace booster_stage {
struct Levels {
  using ValueType = Range;
  enum { Bipolar, Unipolar };
  static auto constexpr selection = Unipolar;
  static auto constexpr unit = " V";

  static inline auto items() -> std::vector<ValueType> const & {
    static auto const items =
        std::vector<ValueType>{bipolar_signal_range, unipolar_signal_range};
    return items;
  }

  static inline auto range() -> Range { return items()[selection]; };

  static inline auto labels() -> std::vector<std::string> const & {
    static auto const labels = std::vector<std::string>{"±5 V", "0–10 V"};
    return labels;
  }

  static inline auto frame_names() -> std::vector<std::string> const & {
    static auto const names = numbered_frame_names("toggle-2", 2);
    return names;
  }
};
} // namespace booster_stage
} // namespace envelope
} // namespace dhe
