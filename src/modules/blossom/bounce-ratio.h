#pragma once

#include "components/range.h"

#include <array>
#include <cmath>

namespace dhe {
namespace blossom {

enum class BounceRatioMode { Quantized, Free };

namespace bounce_ratio {
static auto constexpr size = 2;
static auto range = Range{1.F, 17.F};
static auto constexpr labels =
    std::array<char const *, size>{"Quantized", "Free"};

static auto scale(float normalized, BounceRatioMode mode) -> float {
  return mode == BounceRatioMode::Quantized
             ? std::roundf(range.scale(normalized))
             : range.scale(normalized);
}

static auto normalize(float scaled) -> float { return range.normalize(scaled); }

} // namespace bounce_ratio

struct BounceRatio {
  static auto scale(float normalized, BounceRatioMode mode) -> float {
    return bounce_ratio::scale(normalized, mode);
  }

  static auto normalize(float scaled) -> float {
    return bounce_ratio::normalize(scaled);
  }
};

} // namespace blossom
} // namespace dhe
