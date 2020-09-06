#pragma once

#include "modules/curve-sequencer/status.h"
#include <ostream>

namespace test {
namespace curve_sequencer {
using dhe::curve_sequencer::GeneratorStatus;
using dhe::curve_sequencer::StepStatus;

static auto name_of(dhe::curve_sequencer::GeneratorStatus status)
    -> char const * {
  switch (status) {
  case GeneratorStatus::Completed:
    return "GeneratorStatus::Completed";
  case GeneratorStatus::Generating:
    return "GeneratorStatus::Generating";
  }
}

static auto name_of(dhe::curve_sequencer::StepStatus status) -> char const * {
  switch (status) {
  case StepStatus::Completed:
    return "StepStatus::Completed";
  case StepStatus::InProgress:
    return "StepStatus::InProgress";
  }
}

} // namespace curve_sequencer
} // namespace test

namespace dhe {
namespace curve_sequencer {
static inline auto operator<<(std::ostream &os,
                              dhe::curve_sequencer::StepStatus status)
    -> std::ostream & {
  os << test::curve_sequencer::name_of(status);
  return os;
}

static inline auto operator<<(std::ostream &os,
                              dhe::curve_sequencer::GeneratorStatus status)
    -> std::ostream & {
  os << test::curve_sequencer::name_of(status);
  return os;
}

} // namespace curve_sequencer
} // namespace dhe
