#pragma once

#include <array>

namespace dhe {
namespace envelope {
namespace hostage {
namespace internal {
namespace mode {
static auto constexpr labels = std::array<char const *, 2>{"Hold", "Sustain"};
}
} // namespace internal

enum class Mode { Hold, Sustain };

struct Modes {
  using ValueType = Mode;
  static auto constexpr size = 2;
  static auto constexpr &labels = internal::mode::labels;
};

} // namespace hostage
} // namespace envelope
} // namespace dhe
