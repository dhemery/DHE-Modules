#pragma once

#include "modules/curve-sequencer/interrupt-mode.h"
#include <ostream>

namespace dhe {
namespace curve_sequencer {

static inline auto name_of(InterruptMode mode) -> char const * {
  switch (mode) {
  case InterruptMode::Advance:
    return "InterruptMode::Advance";
  case InterruptMode::Ignore:
    return "InterruptMode::Ignore";
  }
}

static inline auto operator<<(std::ostream &os, InterruptMode mode)
    -> std::ostream & {
  os << name_of(mode);
  return os;
}

} // namespace curve_sequencer
} // namespace dhe
