#pragma once

#include <algorithm>

namespace dhe {
namespace fuzzy_logic {

struct FuzzyResult {
  float a_and_b_;
  float a_or_b_;
  float a_xor_b_;
  float a_implies_b_;
  float b_implies_a_;
};

struct HEngine {
  static inline auto compute(float a, float b) -> FuzzyResult {
    auto const not_a = 10.F - a;
    auto const not_b = 10.F - b;
    auto const a_and_b = a * b * 0.1F;
    auto const a_or_b = a + b - a_and_b;
    return FuzzyResult{
        .a_and_b_ = a_and_b,
        .a_or_b_ = a_or_b,
        .a_xor_b_ = a_or_b - a_and_b,
        .a_implies_b_ = not_a + a_and_b,
        .b_implies_a_ = not_b + a_and_b,
    };
  }
};

struct ZEngine {
  static inline auto compute(float a, float b) -> FuzzyResult {
    auto const not_a = 10.F - a;
    auto const not_b = 10.F - b;
    auto const a_and_b = std::min(a, b);
    auto const a_or_b = std::max(a, b);
    return FuzzyResult{
        .a_and_b_ = a_and_b,
        .a_or_b_ = a_or_b,
        .a_xor_b_ = a_or_b - a_and_b,
        .a_implies_b_ = std::max(not_a, b),
        .b_implies_a_ = std::max(a, not_b),
    };
  }
};
} // namespace fuzzy_logic
} // namespace dhe
