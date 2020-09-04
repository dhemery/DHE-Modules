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

} // namespace curve_sequencer
} // namespace dhe
