#pragma once

#include <string>
#include <vector>

namespace dhe {
namespace envelope {
namespace hostage {

enum class Mode { Hold, Sustain };

struct Modes {
  using ValueType = Mode;
  static auto constexpr size = 2;
  static inline auto labels() -> std::vector<std::string> {
    return std::vector<std::string>{"Hold", "Sustain"};
  }
};

} // namespace hostage
} // namespace envelope
} // namespace dhe
