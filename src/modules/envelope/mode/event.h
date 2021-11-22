#pragma once

#include <array>
#include <ostream>

namespace dhe {
namespace envelope {
namespace mode {

enum class Event { Generated, Completed };

namespace event {
static auto constexpr size = 2;
static auto constexpr names =
    std::array<char const *, size>{"Generated", "Completed"};
} // namespace event

inline auto operator<<(std::ostream &os, Event e) -> std::ostream & {
  auto const index = static_cast<size_t>(e);
  if (index >= event::size) {
    return os << "Unknown Event " << index;
  }
  return os << event::names[index];
}
} // namespace mode
} // namespace envelope
} // namespace dhe
