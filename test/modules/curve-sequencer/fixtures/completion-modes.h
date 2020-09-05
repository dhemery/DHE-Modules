#pragma once

#include "modules/curve-sequencer/completion-mode.h"
#include <array>
#include <ostream>

namespace test {
namespace curve_sequencer {
using dhe::curve_sequencer::CompletionMode;

static auto constexpr completion_modes = std::array<CompletionMode, 2>{
    CompletionMode::Advance,
    CompletionMode::Sustain,
};

static inline auto name_of(CompletionMode mode) -> char const * {
  switch (mode) {
  case CompletionMode::Advance:
    return "CompletionMode::Advance";
  case CompletionMode::Sustain:
    return "CompletionMode::Sustain";
  }
}

} // namespace curve_sequencer
} // namespace test
namespace dhe {
namespace curve_sequencer {

static inline auto operator<<(std::ostream &os, CompletionMode mode)
    -> std::ostream & {
  os << test::curve_sequencer::name_of(mode);
  return os;
}

} // namespace curve_sequencer
} // namespace dhe
