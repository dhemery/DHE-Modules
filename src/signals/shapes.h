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

  static inline auto labels() -> std::vector<std::string> {
    return std::vector<std::string>{"J", "S"};
  }
};
} // namespace dhe
