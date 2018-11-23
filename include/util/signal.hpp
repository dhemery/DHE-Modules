#pragma once
#include "util/range.hpp"

namespace DHE {
namespace Signal {
auto constexpr unipolar_range{Range{0.f, 10.f}};
auto constexpr bipolar_range{Range{-5.f, 5.f}};
} // namespace Signal
} // namespace DHE
