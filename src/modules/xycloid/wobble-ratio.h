#pragma once

#include "components/range.h"
#include "components/sigmoid.h"

#include <array>
#include <cmath>

namespace dhe {
namespace xycloid {

enum class WobbleRatioMode { Quantized, Free };

enum class WobbleRatioRangeId { Inward, InwardOutward, Outward };

namespace wobble_ratio {
static auto constexpr mode_labels =
    std::array<char const *, 2>{"Quantized", "Free"};

static auto constexpr max_ratio = 16.F;
static auto constexpr ranges =
    std::array<Range, 3>{Range{0.F, -max_ratio}, Range{-max_ratio, max_ratio},
                         Range{0.F, max_ratio}};
static auto constexpr range_labels =
    std::array<char const *, ranges.size()>{"In", "-In +Out", "Out"};
} // namespace wobble_ratio

struct WobbleRatio {
  static inline auto scale(float normalized, WobbleRatioRangeId range_id,
                           WobbleRatioMode mode) -> float {
    auto const ratio = range(range_id).scale(normalized);
    return mode == WobbleRatioMode::Quantized ? std::roundf(ratio) : ratio;
  }

  static inline auto normalize(float scaled, WobbleRatioRangeId range_id)
      -> float {
    return range(range_id).normalize(scaled);
  }

  static inline auto range(WobbleRatioRangeId id) -> Range {
    return wobble_ratio::ranges[static_cast<size_t>(id)];
  }
};
} // namespace xycloid
} // namespace dhe
