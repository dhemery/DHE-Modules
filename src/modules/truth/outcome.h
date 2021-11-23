#pragma once

#include <array>
#include <string>
#include <vector>

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
  static auto constexpr size = outcome::size;
  static constexpr auto stepper_slug = "outcome";
  static inline auto labels() -> std::vector<std::string> {
    return {outcome::names.cbegin(), outcome::names.cend()};
  }
};
} // namespace truth
} // namespace dhe
