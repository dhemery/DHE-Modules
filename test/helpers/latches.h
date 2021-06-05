#pragma once
#include "components/latch.h"

#include <ostream>
#include <string>

namespace test {
using Latch = dhe::Latch;
static auto constexpr rising_latch = Latch{true, true};
static auto constexpr falling_latch = Latch{false, true};
static auto constexpr high_latch = Latch{true, false};
static auto constexpr low_latch = Latch{false, false};

static inline auto name_of(Latch const &latch) -> std::string {
  if (latch == test::rising_latch) {
    return "Latch::Rising";
  } else if (latch == test::falling_latch) {
    return "Latch::Falling";
  } else if (latch == test::high_latch) {
    return "Latch::High";
  }
  return "Latch::Low";
}
} // namespace test

namespace dhe {

static inline auto operator<<(std::ostream &os, Latch const &latch)
    -> std::ostream & {
  os << test::name_of(latch);
  return os;
}

} // namespace dhe
