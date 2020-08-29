#pragma once

#include "types/enums.h"
#include <ostream>

namespace dhe {
namespace curve_sequencer_2 {
enum class Source { Level, In, Out };

static auto constexpr source_count = static_cast<int>(Source::Out) + 1;

static inline auto operator<<(std::ostream &os,
                              dhe::curve_sequencer_2::Source source)
    -> std::ostream & {
  os << enum_index(source);
  return os;
}
} // namespace curve_sequencer_2
} // namespace dhe
