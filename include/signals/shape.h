#pragma once

#include <array>
#include <cstdint>

#include "components/range.h"

namespace dhe {
namespace shape {}

struct Shape {
  enum class Id : std::uint8_t { J, S };
  static auto constexpr labels = std::array<char const *, 2>{"J", "S"};

  virtual auto apply(float input, float curvature) const -> float = 0;
  virtual auto invert(float input, float curvature) const -> float = 0;

  static auto by_id(Id id) -> Shape const &;
};

struct JShape : Shape {
  auto apply(float input, float curvature) const -> float override;
  auto invert(float input, float curvature) const -> float override;
};

struct SShape : Shape {
  auto apply(float input, float curvature) const -> float override;
  auto invert(float input, float curvature) const -> float override;
};

static Shape const &j_shape = JShape{};
static Shape const &s_shape = SShape{};

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
