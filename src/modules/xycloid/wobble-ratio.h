#pragma once

#include "components/range.h"

#include <array>
#include <cmath>
#include <cstdint>

namespace dhe {
namespace xycloid {

enum class WobbleRatioMode : std::int8_t { Quantized, Free };

enum class WobbleRatioRangeId : std::int8_t { Inward, InwardOutward, Outward };

namespace wobble_ratio {
static auto constexpr mode_labels =
    std::array<char const *, 2>{"Quantized", "Free"};

static auto constexpr max_ratio = 16.F;
static auto constexpr range_count = 3;
static auto ranges = std::array<Range, range_count>{
    Range{0.F, -max_ratio}, Range{-max_ratio, max_ratio},
    Range{0.F, max_ratio}};
static auto constexpr range_labels =
    std::array<char const *, range_count>{"In", "-In +Out", "Out"};
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
