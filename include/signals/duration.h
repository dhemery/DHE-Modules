#pragma once

#include "components/range.h"

#include <cstdint>
#include <string>
#include <vector>

namespace dhe {
struct DurationCurve {
  static auto constexpr count = 3;
  enum class Id : std::int8_t { Short, Medium, Long };

  static auto by_id(Id) -> DurationCurve const &;
  static auto labels() -> std::vector<std::string> const &;

  DurationCurve(Range range, std::string label);
  auto scale(float rotation) const -> float;
  auto normalize(float seconds) const -> float;
  auto range() const -> Range const &;
  auto label() const -> std::string const &;

private:
  Range const range_;
  std::string const label_;
};

static auto short_duration_curve =
    DurationCurve{Range{0.001F, 1.F}, "0.001-1.0 s"};
static auto medium_duration_curve =
    DurationCurve{Range{0.01F, 10.F}, "0.01-10.0 s"};
static auto long_duration_curve =
    DurationCurve{Range{0.1F, 100.F}, "0.1-100.0 s"};

} // namespace dhe
