#pragma once

#include "modules/curve-sequencer/anchor-mode.h"
#include <array>
#include <ostream>

namespace test {
namespace curve_sequencer {
using dhe::curve_sequencer::AnchorMode;

static auto constexpr anchor_modes = std::array<AnchorMode, 2>{
    AnchorMode::Snap,
    AnchorMode::Track,
};

static auto name_of(dhe::curve_sequencer::AnchorMode mode) -> char const * {
  switch (mode) {
  case AnchorMode::Snap:
    return "AnchorMode::Snap";
  case AnchorMode::Track:
    return "AnchorMode::Track";
  }
}

} // namespace curve_sequencer
} // namespace test

namespace dhe {
namespace curve_sequencer {

static inline auto operator<<(std::ostream &os, AnchorMode mode)
    -> std::ostream & {
  os << test::curve_sequencer::name_of(mode);
  return os;
}

} // namespace curve_sequencer
} // namespace dhe
