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

namespace outcome {
static auto constexpr size = 4;
static auto constexpr names =
    std::array<char const *, size>{"True", "False", "Q", "¬Q"};
} // namespace outcome

struct Outcomes {
  using ValueType = Outcome;
  static auto constexpr &labels = outcome::names;
  static auto constexpr size = outcome::size;
  static auto constexpr stepper_slug = "outcome";
};
} // namespace truth
} // namespace dhe
