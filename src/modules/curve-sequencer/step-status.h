#pragma once
#include <ostream>

namespace dhe {
namespace curve_sequencer {
enum class StepStatus { InProgress, Completed };

static inline auto operator<<(std::ostream &os, StepStatus status)
    -> std::ostream & {
  os << static_cast<int>(status);
  return os;
}
} // namespace curve_sequencer
} // namespace dhe
