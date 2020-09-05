#pragma once

#include "components/latch.h"

namespace test {
using Latch = dhe::Latch;
static auto constexpr rising_latch = Latch{true, true};
static auto constexpr falling_latch = Latch{false, true};
static auto constexpr high_latch = Latch{true, false};
static auto constexpr low_latch = Latch{false, false};
} // namespace test

namespace dhe {

static inline auto operator<<(std::ostream &os, Latch const &latch)
    -> std::ostream & {
  os << "Latch{" << latch.is_high() << ',' << latch.is_edge() << '}';
  return os;
}

} // namespace dhe
