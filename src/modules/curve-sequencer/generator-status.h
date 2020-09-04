#pragma once
#include <ostream>

namespace dhe {
namespace curve_sequencer {
enum class GeneratorStatus { Generating, Completed };

static inline auto operator<<(std::ostream &os, GeneratorStatus status)
    -> std::ostream & {
  os << static_cast<int>(status);
  return os;
}
} // namespace curve_sequencer
} // namespace dhe
