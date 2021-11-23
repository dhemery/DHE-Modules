#pragma once

#include "components/latch.h"

#include <array>
#include <ostream>
#include <string>
#include <vector>

namespace dhe {
namespace sequencizer {

enum class InterruptMode { No, Yes };

namespace interrupt_mode {
static auto constexpr size = 2;
static auto constexpr labels = std::array<char const *, size>{
    "Ignore triggers while generating",
    "Interrupt if triggered",
};
static auto constexpr names = std::array<char const *, size>{
    "InterruptMode::No",
    "InterruptMode::Yes",
};
static auto constexpr values = std::array<InterruptMode, 2>{
    InterruptMode::No,
    InterruptMode::Yes,
};
} // namespace interrupt_mode

struct InterruptModes {
  using ValueType = InterruptMode;
  static auto constexpr size = interrupt_mode::size;
  static auto constexpr stepper_slug = "interrupt-mode";

  static inline auto labels() -> std::vector<std::string> {
    return {interrupt_mode::labels.cbegin(), interrupt_mode::labels.cend()};
  }
};

template <typename Signals> struct Interrupter {
  Interrupter(Signals &signals) : signals_{signals} {}

  auto is_interrupted(int step, Latch const &latch) -> bool {
    return (signals_.interrupt_mode(step) == InterruptMode::Yes) &&
           is_triggered(signals_.trigger_mode(step), latch);
  }

private:
  Signals &signals_;
};

static inline auto operator<<(std::ostream &os, InterruptMode mode)
    -> std::ostream & {
  auto const index = static_cast<size_t>(mode);
  if (index >= interrupt_mode::size) {
    return os << "Unknown InterruptMode " << index;
  }
  return os << interrupt_mode::names[index];
}
} // namespace sequencizer
} // namespace dhe
