#pragma once
#include "./anchor.h"

#include <string>
#include <vector>

namespace dhe {
namespace scannibal {
struct AnchorModes {
  using ValueT = AnchorMode;
  static constexpr auto frame_prefix = "anchor-mode";
  static inline auto labels() -> std::vector<std::string> {
    static const auto labels =
        std::vector<std::string>{"Sample the source", "Track the source"};
    return labels;
  }
};

struct AnchorSources {
  using ValueT = AnchorSource;
  static constexpr auto frame_prefix = "anchor-source";
  static inline auto labels() -> std::vector<std::string> {
    static const auto labels =
        std::vector<std::string>{"Level", "A", "B", "C", "Out"};
    return labels;
  }
};

struct Shapes {
  using ValueT = enum { J, S };
  static constexpr auto frame_prefix = "shape";
  static inline auto labels() -> std::vector<std::string> {
    static const auto labels = std::vector<std::string>{"J", "S"};
    return labels;
  }
};
} // namespace scannibal
} // namespace dhe
