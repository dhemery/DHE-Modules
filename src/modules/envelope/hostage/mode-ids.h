#pragma once

#include <array>

namespace dhe {
namespace envelope {
namespace hostage {

enum class ModeId { Hold, Sustain };

namespace mode {
static auto constexpr labels = std::array<char const *, 2>{"Hold", "Sustain"};
} // namespace mode

} // namespace hostage
} // namespace envelope
} // namespace dhe
