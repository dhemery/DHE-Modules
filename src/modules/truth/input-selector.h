#pragma once

#include <array>

namespace dhe {
namespace truth {
enum class Input0Selection {
  A,
  Q,
};

static auto constexpr input_0_selection_count =
    static_cast<int>(Input0Selection::Q) + 1;

static auto constexpr input_0_selection_descriptions =
    std::array<char const *, input_0_selection_count>{"A", "Q"};
} // namespace truth
} // namespace dhe
