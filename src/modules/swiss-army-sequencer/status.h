#pragma once

namespace dhe {
namespace swiss_army_sequencer {
enum class GeneratorStatus {
  Generating,
  Completed,
};

auto constexpr generator_status_count =
    static_cast<int>(GeneratorStatus::Completed) + 1;

enum class StepStatus {
  InProgress,
  Completed,
};

auto constexpr step_status_count = static_cast<int>(StepStatus::Completed) + 1;

} // namespace swiss_army_sequencer
} // namespace dhe
