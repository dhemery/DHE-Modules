#pragma once

#include "components/latch.h"

namespace dhe {
namespace curve_sequencer_2 {
enum class TriggerMode {
  GateRises,
  GateFalls,
  GateChanges,
  GateIsHigh,
  GateIsLow
};

static auto constexpr trigger_mode_count =
    static_cast<int>(TriggerMode::GateIsLow) + 1;

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

template <typename Controls> class Interrupter {
public:
  Interrupter(Controls &controls) : controls_{controls} {}

  auto is_interrupted(int step, Latch const &latch) -> bool {
    return controls_.interrupt_on_trigger(step) &&
           is_triggered(controls_.trigger_mode(step), latch);
  }

private:
  Controls &controls_;
};

template <typename Controls> class Sustainer {
public:
  Sustainer(Controls &controls) : controls_{controls} {}

  auto is_done(int step, Latch const &latch) -> bool {
    return controls_.advance_on_end_of_curve(step) ||
           is_triggered(controls_.trigger_mode(step), latch);
  }

private:
  Controls &controls_;
};

} // namespace curve_sequencer_2
} // namespace dhe
