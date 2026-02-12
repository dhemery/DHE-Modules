#pragma once

#include "components/range.h"

#include <cstdint>
#include <vector>

namespace dhe {

struct Shape {
  static auto constexpr count = 2;
  enum class Id : std::int8_t { J, S };

  static auto get(Id id) -> Shape const &;
  static auto labels() -> std::vector<std::string> const &;

  virtual auto apply(float input, float curvature) const -> float = 0;
  virtual auto invert(float input, float curvature) const -> float = 0;
};

struct JShape : Shape {
  auto apply(float input, float curvature) const -> float override;
  auto invert(float input, float curvature) const -> float override;
};

struct SShape : Shape {
  auto apply(float input, float curvature) const -> float override;
  auto invert(float input, float curvature) const -> float override;
};

static auto const j_shape = JShape{};
static auto const s_shape = SShape{};

namespace curvature {
static auto range = Range{-0.9999F, 0.9999F};
} // namespace curvature

struct Curvature {
  static auto constexpr &range = curvature::range;
  /**
   * This curvature gives a curvature knob a gentle inverted S taper, increasing
   * sensitivity in the middle of the knob normalize and decreasing sensitivity
   * toward the extremes.
   */
  static auto constexpr taper_curvature = 0.65F;

  static auto scale(float normalized) -> float;

  static auto normalize(float scaled) -> float;
};

} // namespace dhe
