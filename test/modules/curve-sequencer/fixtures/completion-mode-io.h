#pragma once

#include "modules/curve-sequencer/completion-mode.h"
#include <ostream>

namespace dhe {
namespace curve_sequencer {

static inline auto name_of(CompletionMode mode) -> char const * {
  switch (mode) {
  case CompletionMode::Advance:
    return "CompletionMode::Advance";
  case CompletionMode::Sustain:
    return "CompletionMode::Sustain";
  }
}

static inline auto operator<<(std::ostream &os, CompletionMode mode)
    -> std::ostream & {
  os << name_of(mode);
  return os;
}

} // namespace curve_sequencer
} // namespace dhe
