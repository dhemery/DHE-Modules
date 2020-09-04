#pragma once

#include "modules/curve-sequencer/anchor-mode.h"
#include <ostream>

namespace dhe {
namespace curve_sequencer {

static auto name_of(dhe::curve_sequencer::AnchorMode mode) -> char const * {
  switch (mode) {
  case AnchorMode::Snap:
    return "AnchorMode::Snap";
  case AnchorMode::Track:
    return "AnchorMode::Track";
  }
}
static inline auto operator<<(std::ostream &os,
                              dhe::curve_sequencer::AnchorMode mode)
    -> std::ostream & {
  os << name_of(mode);
  return os;
}

} // namespace curve_sequencer
} // namespace dhe
