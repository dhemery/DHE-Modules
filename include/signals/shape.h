#pragma once

#include <cstdint>
#include <string>
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
} // namespace dhe
