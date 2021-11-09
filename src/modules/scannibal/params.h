#pragma once
#include "./anchor.h"

#include <string>
#include <vector>

namespace dhe {
namespace scannibal {
struct AnchorModes {
  using ValueType = AnchorMode;
  static constexpr auto stepper_slug = "anchor-mode";
  static inline auto labels() -> std::vector<std::string> {
    static const auto labels =
        std::vector<std::string>{"Sample the source", "Track the source"};
    return labels;
  }
};

struct AnchorSources {
  using ValueType = AnchorSource;
  static constexpr auto stepper_slug = "anchor-source";
  static inline auto labels() -> std::vector<std::string> {
    static const auto labels =
        std::vector<std::string>{"Level", "A", "B", "C", "Out"};
    return labels;
  }
};
} // namespace scannibal
} // namespace dhe
