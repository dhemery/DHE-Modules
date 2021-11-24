#pragma once

#include "components/range.h"

#include <array>
#include <cmath>
#include <string>
#include <vector>

namespace dhe {
namespace blossom {

enum class BounceRatioMode { Quantized, Free };

namespace bounce_ratio {
static auto constexpr size = 2;
static auto constexpr range = Range{1.F, 17.F};
static auto constexpr names =
    std::array<char const *, size>{"Quantized", "Free"};

static constexpr auto scale(float normalized, BounceRatioMode mode) -> float {
  return mode == BounceRatioMode::Quantized
             ? std::roundf(range.scale(normalized))
             : range.scale(normalized);
}

static constexpr auto normalize(float scaled) -> float {
  return range.normalize(scaled);
}

struct KnobMap {
  static auto constexpr default_value = 9.F;
  static auto constexpr unit = "x";

  auto to_display(float value) const -> float { return scale(value, mode_); }

  static constexpr auto to_value(float display) -> float {
    return normalize(display);
  }

  void select_mode(BounceRatioMode mode) { mode_ = mode; }

private:
  BounceRatioMode mode_{BounceRatioMode::Free};
};

} // namespace bounce_ratio

struct BounceRatioModes {
  static auto constexpr size = bounce_ratio::size;
  using ValueType = BounceRatioMode;

  static inline auto labels() -> std::vector<std::string> {
    return {bounce_ratio::names.cbegin(), bounce_ratio::names.cend()};
  }
};

struct BounceRatio {
  using KnobMap = bounce_ratio::KnobMap;

  static constexpr auto scale(float normalized, BounceRatioMode mode) -> float {
    return bounce_ratio::scale(normalized, mode);
  }

  static constexpr auto normalize(float scaled) -> float {
    return bounce_ratio::normalize(scaled);
  }
};

} // namespace blossom
} // namespace dhe
