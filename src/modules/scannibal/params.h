#pragma once

#include "anchor.h"
#include "signals/enums.h"

#include <string>
#include <vector>

namespace dhe {
namespace scannibal {
struct AnchorModes : Enums<AnchorMode, 2> {
  static constexpr auto stepper_slug = "anchor-mode";
  static inline auto labels() -> std::vector<std::string> const & {
    static const auto labels =
        std::vector<std::string>{"Sample the source", "Track the source"};
    return labels;
  }
};

struct AnchorSources : Enums<AnchorSource, 5> {
  static constexpr auto stepper_slug = "anchor-source";
  static inline auto labels() -> std::vector<std::string> const & {
    static const auto labels =
        std::vector<std::string>{"Level", "A", "B", "C", "Out"};
    return labels;
  }
};
} // namespace scannibal
} // namespace dhe
