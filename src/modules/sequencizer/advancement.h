#pragma once

#include "components/latch.h"
#include <array>

namespace dhe {
namespace sequencizer {

enum class SustainMode { No, Yes };
static auto constexpr sustain_mode_count =
    static_cast<int>(SustainMode::Yes) + 1;

enum class InterruptMode { No, Yes };
static auto constexpr interrupt_mode_count =
    static_cast<int>(InterruptMode::Yes) + 1;

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

template <typename Signals> class Interrupter {
public:
  Interrupter(Signals &signals) : signals_{signals} {}

  auto is_interrupted(int step, Latch const &latch) -> bool {
    return (signals_.interrupt_mode(step) == InterruptMode::Yes) &&
           is_triggered(signals_.trigger_mode(step), latch);
  }

private:
  Signals &signals_;
};

template <typename Signals> class Sustainer {
public:
  Sustainer(Signals &signals) : signals_{signals} {}

  auto is_done(int step, Latch const &latch) -> bool {

    return (signals_.completion_mode(step) == SustainMode::No) ||
           is_triggered(signals_.trigger_mode(step), latch);
  }

private:
  Signals &signals_;
};

} // namespace sequencizer
} // namespace dhe
