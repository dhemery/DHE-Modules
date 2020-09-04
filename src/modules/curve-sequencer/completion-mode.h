#pragma once

#include <ostream>

namespace dhe {
namespace curve_sequencer {

enum class CompletionMode { Sustain, Advance };

static inline auto name_of(CompletionMode mode) -> char const * {
  return mode == CompletionMode::Sustain ? "CompletionMode::Sustain"
                                         : "CompletionMode::Advance";
}

static inline auto operator<<(std::ostream &os, CompletionMode mode)
    -> std::ostream & {
  os << name_of(mode);
  return os;
}

} // namespace curve_sequencer
} // namespace dhe
