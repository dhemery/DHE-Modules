#pragma once

#include "types/enums.h"
#include <ostream>

namespace dhe {
namespace curve_sequencer {
enum class StepEvent { Generated, Completed };
static inline auto operator<<(std::ostream &os, StepEvent event)
    -> std::ostream & {
  os << enum_index(event);
  return os;
}

} // namespace curve_sequencer
} // namespace dhe
