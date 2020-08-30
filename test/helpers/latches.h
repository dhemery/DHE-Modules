#pragma once

#include "components/latch.h"

namespace test {
using Latch = dhe::Latch;
static auto constexpr rising_latch = Latch{true, true};
static auto constexpr falling_latch = Latch{false, true};
static auto constexpr high_latch = Latch{true, false};
static auto constexpr low_latch = Latch{false, false};
} // namespace test
