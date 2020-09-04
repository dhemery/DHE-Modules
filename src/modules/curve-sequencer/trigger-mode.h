#pragma once

#include "components/latch.h"
#include <array>

namespace dhe {
namespace curve_sequencer {
enum class TriggerMode {
  GateRises,
  GateFalls,
  GateChanges,
  GateIsHigh,
  GateIsLow
};

static auto constexpr trigger_mode_count =
    static_cast<int>(TriggerMode::GateIsLow) + 1;

static auto constexpr trigger_mode_descriptions =
    std::array<char const *, trigger_mode_count>{"Gate rises", "Gate falls",
                                                 "Gate rises or falls",
                                                 "Gate is high", "Gate is low"};

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

static auto constexpr trigger_mode_names =
    std::array<char const *, trigger_mode_count>{
        "TriggerMode::GateRises", "TriggerMode::GateFalls",
        "TriggerMode::GateChanges", "TriggerMode::GateIsHigh",
        "TriggerMode::GateIsLow"};

static inline auto name_of(TriggerMode mode) -> const char * {
  return trigger_mode_names[static_cast<int>(mode)];
}

static inline auto operator<<(std::ostream &os, TriggerMode mode)
    -> std::ostream & {
  os << name_of(mode);
  return os;
}

} // namespace curve_sequencer
} // namespace dhe
