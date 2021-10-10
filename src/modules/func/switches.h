#pragma once
#include "./operation.h"

#include <string>
#include <vector>

namespace dhe {
namespace func {

struct OffsetRanges {
  using ValueT = int;
  static constexpr auto frame_prefix = "offset-range";
  static inline auto labels() -> std::vector<std::string> const & {
    static auto const labels =
        std::vector<std::string>{"0–5 V", "±5 V", "0–10 V", "±10 V"};
    return labels;
  }
};

struct MultiplierRanges {
  using ValueT = int;
  static constexpr auto frame_prefix = "multiplier-range";
  static inline auto labels() -> std::vector<std::string> const & {
    static auto const labels =
        std::vector<std::string>{"0–1", "±1", "0–2", "±2"};
    return labels;
  }
};

struct Operators {
  using ValueT = Operation;
  static constexpr auto frame_prefix = "toggle-2";
  static inline auto labels() -> std::vector<std::string> const & {
    static const auto labels =
        std::vector<std::string>{"Add (offset)", "Multiply (scale)"};
    return labels;
  }
};
} // namespace func

} // namespace dhe
