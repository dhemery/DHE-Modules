#pragma once
#include "./anchor.h"

#include <string>
#include <vector>

namespace dhe {
namespace scannibal {
struct AnchorModes {
  using value_type = AnchorMode; // NOLINT
  static constexpr auto stepper_slug = "anchor-mode";
  static inline auto labels() -> std::vector<std::string> {
    static const auto labels =
        std::vector<std::string>{"Sample the source", "Track the source"};
    return labels;
  }
};

struct AnchorSources {
  using value_type = AnchorSource; // NOLINT
  static constexpr auto stepper_slug = "anchor-source";
  static inline auto labels() -> std::vector<std::string> {
    static const auto labels =
        std::vector<std::string>{"Level", "A", "B", "C", "Out"};
    return labels;
  }
};
} // namespace scannibal
} // namespace dhe
