#pragma once

#include <vector>

#include "range.h"

namespace DHE {
namespace Signal {

static constexpr auto bipolar_range = Range{-5.f, 5.f};
static constexpr auto unipolar_range = Range{0.f, 10.f};

auto range(bool is_uni) -> Range const &;

auto ranges() -> std::array<Range const *, 2> const &;
} // namespace Signal
} // namespace DHE
