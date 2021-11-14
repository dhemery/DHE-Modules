#pragma once

#include "signals/enums.h"

namespace dhe {
namespace envelope {
namespace hostage {

enum class Mode { Hold, Sustain };

struct Modes : Enums<Mode, 2> {
  static inline auto labels() -> std::vector<std::string> const & {
    static auto const labels = std::vector<std::string>{"Hold", "Sustain"};
    return labels;
  }
};

} // namespace hostage
} // namespace envelope
} // namespace dhe
