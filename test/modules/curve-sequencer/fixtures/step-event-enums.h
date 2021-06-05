#pragma once

#include "modules/curve-sequencer/step-event.h"
#include <ostream>
#include <string>

namespace test {
namespace curve_sequencer {
using dhe::curve_sequencer::StepEvent;

static inline auto name_of(dhe::curve_sequencer::StepEvent event)
    -> std::string {
  switch (event) {
  case StepEvent::Generated:
    return "StepEvent::Generated";
  case StepEvent::Completed:
    return "StepEvent::Completed";
  default:
    return "Unknown StepEvent " + std::to_string(static_cast<int>(event));
  }
}

} // namespace curve_sequencer
} // namespace test

namespace dhe {
namespace curve_sequencer {
static inline auto operator<<(std::ostream &os,
                              dhe::curve_sequencer::StepEvent status)
    -> std::ostream & {
  os << test::curve_sequencer::name_of(status);
  return os;
}

} // namespace curve_sequencer
} // namespace dhe
