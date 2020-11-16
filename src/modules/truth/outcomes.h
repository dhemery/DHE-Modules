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
} // namespace truth
} // namespace dhe
