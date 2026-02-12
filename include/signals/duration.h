#pragma once

#include "components/range.h"

#include <cstdint>

namespace dhe {
struct DurationTaper {
  static auto constexpr count = 3;
  enum class Id : std::int8_t { Short, Medium, Long };

  static auto by_id(Id) -> DurationTaper const &;
  static auto labels() -> std::vector<std::string> const &;
  static auto label(Id) -> std::string;

  DurationTaper(Range const &);
  auto scale(float rotation) const -> float;
  auto normalize(float seconds) const -> float;
  auto range() const -> Range const &;

private:
  Range const range_;
};

static auto short_duration_range = DurationTaper{Range{0.001F, 1.F}};
static auto medium_duration_range = DurationTaper{Range{0.01F, 10.F}};
static auto long_duration_range = DurationTaper{Range{0.1F, 100.F}};

} // namespace dhe
