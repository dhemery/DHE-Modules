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

template <typename Module> class Interrupter {
public:
  Interrupter(Module &module) : module_{module} {}

  auto is_interrupted(int step, Latch const &latch) -> bool {
    return (module_.interrupt_mode(step) == InterruptMode::Yes) &&
           is_triggered(module_.trigger_mode(step), latch);
  }

private:
  Module &module_;
};

template <typename Module> class Sustainer {
public:
  Sustainer(Module &module) : module_{module} {}

  auto is_done(int step, Latch const &latch) -> bool {

    return (module_.completion_mode(step) == SustainMode::No) ||
           is_triggered(module_.trigger_mode(step), latch);
  }

private:
  Module &module_;
};

} // namespace sequencizer
} // namespace dhe
