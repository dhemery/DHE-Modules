#pragma once

#include "util/range.h"

namespace DHE {
namespace Signal {
constexpr auto unipolar_range = Range{0.f, 10.f};
constexpr auto bipolar_range = Range{-5.f, 5.f};
} // namespace Signal
} // namespace DHE
