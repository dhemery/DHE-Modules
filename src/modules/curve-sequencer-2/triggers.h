#pragma once

#include "components/latch.h"
#include "types/enums.h"
#include <array>

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

static auto constexpr trigger_mode_descriptions =
    std::array<char const *, trigger_mode_count>{"Gate rises", "Gate falls",
                                                 "Gate changes", "Gate is high",
                                                 "Gate is low"};

static auto constexpr trigger_mode_names =
    std::array<char const *, dhe::curve_sequencer_2::trigger_mode_count>{
        "GateRises", "GateFalls", "GateChanges", "GateIsHigh", "GateIsLow"};

static auto constexpr trigger_modes =
    std::array<TriggerMode, trigger_mode_count>{
        TriggerMode::GateRises,   TriggerMode::GateFalls,
        TriggerMode::GateChanges, TriggerMode::GateIsHigh,
        TriggerMode::GateIsLow,
    };

static auto name_of(TriggerMode mode) -> char const * {
  return trigger_mode_names[enum_index(mode)];
}

static inline auto operator<<(std::ostream &os,
                              dhe::curve_sequencer_2::TriggerMode mode)
    -> std::ostream & {
  os << name_of(mode);
  return os;
}

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
