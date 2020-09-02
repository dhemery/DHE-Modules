#pragma once

#include <ostream>

namespace dhe {
namespace curve_sequencer {
enum class StepEvent { Generated, Completed };
static inline auto operator<<(std::ostream &os, StepEvent event)
    -> std::ostream & {
  os << static_cast<int>(event);
  return os;
}

} // namespace curve_sequencer
} // namespace dhe
