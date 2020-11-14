#pragma once

#include <array>

namespace dhe {
namespace truth {
enum class Outcome {
  False,
  True,
};

static auto constexpr outcome_count = static_cast<int>(Outcome::True) + 1;

static auto constexpr outcome_descriptions =
    std::array<char const *, outcome_count>{"False", "True"};

enum class Condition {
  InputRises,
  InputFalls,
  InputChanges,
  InputIsHigh,
  InputIsLow,
};

static auto constexpr condition_count =
    static_cast<int>(Condition::InputIsLow) + 1;

static auto constexpr condition_descriptions =
    std::array<char const *, condition_count>{"Input rises", "Input falls",
                                              "Input rises or falls",
                                              "Input is high", "Input is low"};

} // namespace truth
} // namespace dhe
