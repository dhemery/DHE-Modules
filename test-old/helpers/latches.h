#pragma once

#include "components/Latch.h"

#include <vector>

namespace test {
using dhe::Latch;
static auto constexpr risingLatch = Latch{true, true};
static auto constexpr fallingLatch = Latch{false, true};
static auto constexpr highLatch = Latch{true, false};
static auto constexpr lowLatch = Latch{false, false};

auto const allLatches =
    std::vector<Latch>{risingLatch, fallingLatch, lowLatch, highLatch};

} // namespace test
