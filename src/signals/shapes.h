#pragma once

#include "components/sigmoid.h"

#include <string>
#include <vector>

namespace dhe {
struct Shapes {
  using ValueType = Shape::Id;
  static auto constexpr size = 2;
  static auto constexpr unit = "";
  static auto constexpr stepper_slug = "shape";

  static inline auto labels() -> std::vector<std::string> const & {
    static auto const labels = std::vector<std::string>{"J", "S"};
    return labels;
  }
};
} // namespace dhe
