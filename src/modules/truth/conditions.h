#pragma once

#include "components/latch.h"
#include <array>

namespace dhe {
namespace truth {
enum class Condition {
  High,
  Rise,
  Fall,
  Edge,
};

static auto constexpr condition_count = static_cast<int>(Condition::Edge) + 1;

static auto constexpr condition_descriptions =
    std::array<char const *, condition_count>{
        "Input is high", "Input rises", "Input falls", "Input rises or falls"};

static inline auto is_satisfied(Condition condition, dhe::Latch const &input)
    -> bool {
  switch (condition) {
  case Condition::High:
  default:
    return input.is_high();
  case Condition::Rise:
    return input.is_rise();
  case Condition::Fall:
    return input.is_fall();
  case Condition::Edge:
    return input.is_edge();
  }
}
} // namespace truth
} // namespace dhe
