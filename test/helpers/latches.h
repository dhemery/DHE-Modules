#pragma once

#include "components/latch.h"

namespace test {
using Latch = dhe::Latch;
static auto constexpr rising_gate = Latch{true, true};
static auto constexpr falling_gate = Latch{false, true};
static auto constexpr high_gate = Latch{true, false};
static auto constexpr low_gate = Latch{false, false};
} // namespace test
