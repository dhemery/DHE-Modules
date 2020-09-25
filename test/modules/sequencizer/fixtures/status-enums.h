#pragma once

#include "modules/sequencizer/status.h"
#include <array>
#include <ostream>
#include <string>

namespace test {
namespace sequencizer {
using dhe::sequencizer::generator_status_count;
using dhe::sequencizer::GeneratorStatus;
using dhe::sequencizer::step_status_count;
using dhe::sequencizer::StepStatus;

auto constexpr generator_statuses =
    std::array<GeneratorStatus, generator_status_count>{
        GeneratorStatus::Generating,
        GeneratorStatus::Completed,
    };

static inline auto name_of(dhe::sequencizer::GeneratorStatus status)
    -> std::string {
  switch (status) {
  case GeneratorStatus::Generating:
    return "GeneratorStatus::Generating";
  case GeneratorStatus::Completed:
    return "GeneratorStatus::Completed";
  default:
    return "Unknown GeneratorStatus " +
           std::to_string(static_cast<int>(status));
  }
}

auto constexpr step_statuses = std::array<StepStatus, step_status_count>{
    StepStatus::Generating,
    StepStatus::Sustaining,
    StepStatus::Completed,
};

static inline auto name_of(dhe::sequencizer::StepStatus status) -> std::string {
  switch (status) {
  case StepStatus::Generating:
    return "StepStatus::Curving";
  case StepStatus::Sustaining:
    return "StepStatus::Sustaining";
  case StepStatus::Completed:
    return "StepStatus::Completed";
  default:
    return "Unknown StepStatus " + std::to_string(static_cast<int>(status));
  }
}

} // namespace sequencizer
} // namespace test

namespace dhe {
namespace sequencizer {
static inline auto operator<<(std::ostream &os,
                              dhe::sequencizer::StepStatus status)
    -> std::ostream & {
  os << test::sequencizer::name_of(status);
  return os;
}

static inline auto operator<<(std::ostream &os,
                              dhe::sequencizer::GeneratorStatus status)
    -> std::ostream & {
  os << test::sequencizer::name_of(status);
  return os;
}

} // namespace sequencizer
} // namespace dhe
