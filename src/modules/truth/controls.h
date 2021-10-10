#pragma once

#include "./gate-modes.h"
#include "./outcomes.h"

#include <string>
#include <vector>

namespace dhe {
namespace truth {
struct GateModes {
  using ValueT = GateMode;
  static constexpr auto frame_prefix = "gate-mode";
  static inline auto labels() -> std::vector<std::string> const & {
    static const auto labels = std::vector<std::string>{
        "Gate rises", "Gate falls", "Gate rises or falls", "Gate is high",
        "Gate is low"};
    return labels;
  }
};

struct Outcomes {
  using ValueT = Outcome;
  static constexpr auto frame_prefix = "outcome";
  static inline auto labels() -> std::vector<std::string> const & {
    static const auto labels =
        std::vector<std::string>{"True", "False", "Q", "¬Q"};
    return labels;
  }
};

} // namespace truth
} // namespace dhe
