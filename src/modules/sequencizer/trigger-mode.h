#pragma once

#include "components/latch.h"

#include <array>
#include <string>
#include <vector>

namespace dhe {
namespace sequencizer {

enum class TriggerMode {
  GateRises,
  GateFalls,
  GateChanges,
  GateIsHigh,
  GateIsLow
};

namespace trigger_mode {
static auto constexpr size = 5;
static auto constexpr labels = std::array<char const *, size>{
    "Gate rises",   "Gate falls",  "Gate rises or falls",
    "Gate is high", "Gate is low",
};
static auto constexpr names = std::array<char const *, size>{
    "TriggerMode::GateRises",   "TriggerMode::GateFalls",
    "TriggerMode::GateChanges", "TriggerMode::GateIsHigh",
    "TriggerMode::GateIsLow",
};
static auto constexpr values = std::array<TriggerMode, size>{
    TriggerMode::GateRises,  TriggerMode::GateFalls, TriggerMode::GateChanges,
    TriggerMode::GateIsHigh, TriggerMode::GateIsLow,
};
} // namespace trigger_mode

struct TriggerModes {
  using value_type = TriggerMode;
  static auto constexpr size = trigger_mode::size;
  static auto constexpr stepper_slug = "trigger-mode";

  static inline auto labels() -> std::vector<std::string> {
    return {trigger_mode::labels.cbegin(), trigger_mode::labels.cend()};
  }
};

static inline auto is_triggered(TriggerMode mode, dhe::Latch const &gate)
    -> bool {
  switch (mode) {
  case TriggerMode::GateIsHigh:
  default:
    return gate.is_high();
  case TriggerMode::GateIsLow:
    return gate.is_low();
  case TriggerMode::GateRises:
    return gate.is_rise();
  case TriggerMode::GateFalls:
    return gate.is_fall();
  case TriggerMode::GateChanges:
    return gate.is_edge();
  }
}

static inline auto operator<<(std::ostream &os, TriggerMode mode)
    -> std::ostream & {
  auto const index = static_cast<size_t>(mode);
  if (index >= trigger_mode::size) {
    return os << "Unknown TriggerMode " << index;
  }
  return os << trigger_mode::names[index];
}

} // namespace sequencizer
} // namespace dhe
