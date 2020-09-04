#pragma once

#include "modules/curve-sequencer/generator-status.h"
#include <ostream>

namespace dhe {
namespace curve_sequencer {

static auto name_of(dhe::curve_sequencer::GeneratorStatus status)
    -> char const * {
  switch (status) {
  case GeneratorStatus::Completed:
    return "GeneratorStatus::Completed";
  case GeneratorStatus::Generating:
    return "GeneratorStatus::Generating";
  }
}
static inline auto operator<<(std::ostream &os,
                              dhe::curve_sequencer::GeneratorStatus status)
    -> std::ostream & {
  os << name_of(status);
  return os;
}

} // namespace curve_sequencer
} // namespace dhe
