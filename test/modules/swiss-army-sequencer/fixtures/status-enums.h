#pragma once

#include "modules/cv-sequencer/status.h"
#include <array>
#include <ostream>
#include <string>

namespace test {
namespace cv_sequencer {
using dhe::cv_sequencer::generator_status_count;
using dhe::cv_sequencer::GeneratorStatus;
using dhe::cv_sequencer::step_status_count;
using dhe::cv_sequencer::StepStatus;

auto constexpr generator_statuses =
    std::array<GeneratorStatus, generator_status_count>{
        GeneratorStatus::Generating,
        GeneratorStatus::Completed,
    };

static inline auto name_of(dhe::cv_sequencer::GeneratorStatus status)
    -> std::string {
  switch (status) {
  case GeneratorStatus::Completed:
    return "GeneratorStatus::Completed";
  case GeneratorStatus::Generating:
    return "GeneratorStatus::Generating";
  default:
    return "Unknown GeneratorStatus " +
           std::to_string(static_cast<int>(status));
  }
}

auto constexpr step_statuses = std::array<StepStatus, step_status_count>{
    StepStatus::InProgress,
    StepStatus::Completed,
};

static inline auto name_of(dhe::cv_sequencer::StepStatus status)
    -> std::string {
  switch (status) {
  case StepStatus::Completed:
    return "StepStatus::Completed";
  case StepStatus::InProgress:
    return "StepStatus::InProgress";
  default:
    return "Unknown StepStatus " + std::to_string(static_cast<int>(status));
  }
}

} // namespace cv_sequencer
} // namespace test

namespace dhe {
namespace cv_sequencer {
static inline auto operator<<(std::ostream &os,
                              dhe::cv_sequencer::StepStatus status)
    -> std::ostream & {
  os << test::cv_sequencer::name_of(status);
  return os;
}

static inline auto operator<<(std::ostream &os,
                              dhe::cv_sequencer::GeneratorStatus status)
    -> std::ostream & {
  os << test::cv_sequencer::name_of(status);
  return os;
}

} // namespace cv_sequencer
} // namespace dhe
