#pragma once

#include <array>
#include <ostream>

namespace dhe {
namespace curve_sequencer {
enum class StepEvent { Generated, Completed };

namespace step_event {
static auto constexpr size = 2;
static auto constexpr names =
    std::array<char const *, size>{"Generated", "Completed"};

static inline auto name(StepEvent event) -> char const * {
  return names[static_cast<size_t>(event)];
}
} // namespace step_event

static inline auto operator<<(std::ostream &os, StepEvent event)
    -> std::ostream & {
  auto const index = static_cast<size_t>(event);
  if (index >= step_event::size) {
    return os << "Unknown StepEvent " << index;
  }
  return os << step_event::name(event);
}

} // namespace curve_sequencer
} // namespace dhe
