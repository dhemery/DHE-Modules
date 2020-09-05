#pragma once

#include "modules/curve-sequencer/interrupt-mode.h"
#include <array>
#include <ostream>

namespace test {
namespace curve_sequencer {
using dhe::curve_sequencer::InterruptMode;

static auto constexpr interrupt_modes = std::array<InterruptMode, 2>{
    InterruptMode::Ignore,
    InterruptMode::Advance,
};

static inline auto name_of(InterruptMode mode) -> char const * {
  switch (mode) {
  case InterruptMode::Advance:
    return "InterruptMode::Advance";
  case InterruptMode::Ignore:
    return "InterruptMode::Ignore";
  }
}
} // namespace curve_sequencer
} // namespace test

namespace dhe {
namespace curve_sequencer {
static inline auto operator<<(std::ostream &os, InterruptMode mode)
    -> std::ostream & {
  os << test::curve_sequencer::name_of(mode);
  return os;
}
} // namespace curve_sequencer
} // namespace dhe
