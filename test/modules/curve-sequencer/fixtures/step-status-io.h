#pragma once

#include "modules/curve-sequencer/step-status.h"
#include <ostream>

namespace dhe {
namespace curve_sequencer {

static auto name_of(dhe::curve_sequencer::StepStatus status) -> char const * {
  switch (status) {
  case StepStatus::Completed:
    return "StepStatus::Completed";
  case StepStatus::InProgress:
    return "StepStatus::InProgress";
  }
}
static inline auto operator<<(std::ostream &os,
                              dhe::curve_sequencer::StepStatus status)
    -> std::ostream & {
  os << name_of(status);
  return os;
}

} // namespace curve_sequencer
} // namespace dhe
