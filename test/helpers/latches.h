#pragma once
#include "components/latch.h"
#include <ostream>

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
  if (latch == test::rising_latch) {
    os << "Latch::Rising";
  } else if (latch == test::falling_latch) {
    os << "Latch::Falling";
  } else if (latch == test::high_latch) {
    os << "Latch::High";
  } else {
    os << "Latch::Low";
  }
  return os;
}

} // namespace dhe
