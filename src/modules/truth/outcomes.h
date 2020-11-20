#pragma once

#include <array>

namespace dhe {
namespace truth {
enum class Outcome {
  True,
  False,
  Q,
  QNot,
};

static auto constexpr outcome_count = static_cast<int>(Outcome::QNot) + 1;

static auto constexpr outcome_descriptions =
    std::array<char const *, outcome_count>{"True", "False", "Q", "¬Q"};
} // namespace truth
} // namespace dhe
