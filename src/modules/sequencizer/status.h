#pragma once

namespace dhe {
namespace sequencizer {
enum class GeneratorStatus {
  Generating,
  Completed,
};

auto constexpr generator_status_count =
    static_cast<int>(GeneratorStatus::Completed) + 1;

enum class StepStatus {
  Generating,
  Sustaining,
  Idle,
};

auto constexpr step_status_count = static_cast<int>(StepStatus::Idle) + 1;

} // namespace sequencizer
} // namespace dhe
