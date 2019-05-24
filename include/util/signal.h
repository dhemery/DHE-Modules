#pragma once

#include "range.h"

#include <array>

namespace dhe {
namespace signal {

  static constexpr auto bipolarRange = Range{-5.f, 5.f};
  static constexpr auto unipolarRange = Range{0.f, 10.f};

  auto range(bool isUni) -> Range const &;

  auto ranges() -> std::array<Range const *, 2> const &;
} // namespace signal
} // namespace dhe
