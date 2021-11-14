#pragma once

#include "gate-modes.h"
#include "outcomes.h"
#include "signals/enums.h"

#include <string>
#include <vector>

namespace dhe {
namespace truth {
struct GateModes : Enums<GateMode, 5> {
  static constexpr auto stepper_slug = "gate-mode";
  static inline auto labels() -> std::vector<std::string> const & {
    static const auto labels = std::vector<std::string>{
        "Gate rises", "Gate falls", "Gate rises or falls", "Gate is high",
        "Gate is low"};
    return labels;
  }
};

struct Outcomes : Enums<Outcome, 4> {
  static constexpr auto stepper_slug = "outcome";
  static inline auto labels() -> std::vector<std::string> const & {
    static const auto labels =
        std::vector<std::string>{"True", "False", "Q", "¬Q"};
    return labels;
  }
};

} // namespace truth
} // namespace dhe
