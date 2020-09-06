#pragma once

#include "modules/curve-sequencer/completion-mode.h"
#include "modules/curve-sequencer/interrupt-mode.h"
#include "modules/curve-sequencer/trigger-mode.h"
#include <array>
#include <ostream>

namespace test {
namespace curve_sequencer {
using dhe::curve_sequencer::CompletionMode;
using dhe::curve_sequencer::InterruptMode;
using dhe::curve_sequencer::TriggerMode;

static auto constexpr completion_modes = std::array<CompletionMode, 2>{
    CompletionMode::Advance,
    CompletionMode::Sustain,
};

static inline auto name_of(CompletionMode mode) -> char const * {
  switch (mode) {
  case CompletionMode::Advance:
    return "CompletionMode::Advance";
  case CompletionMode::Sustain:
    return "CompletionMode::Sustain";
  }
}

static auto constexpr interrupt_modes = std::array<InterruptMode, 2>{
    InterruptMode::Ignore,
    InterruptMode::Advance,
};

static inline auto name_of(InterruptMode mode) -> char const * {
  switch (mode) {
  case InterruptMode::Advance:
    return "InterruptMode::Advance";
  case InterruptMode::Ignore:
    return "InterruptMode::Ignore";
  }
}

static auto constexpr trigger_modes =
    std::array<TriggerMode, dhe::curve_sequencer::trigger_mode_count>{
        TriggerMode::GateRises,   TriggerMode::GateFalls,
        TriggerMode::GateChanges, TriggerMode::GateIsHigh,
        TriggerMode::GateIsLow,
    };

static auto name_of(dhe::curve_sequencer::TriggerMode mode) -> char const * {
  switch (mode) {
  case TriggerMode::GateRises:
    return "TriggerMode::GateRises";
  case TriggerMode::GateFalls:
    return "TriggerMode::GateFalls";
  case TriggerMode::GateChanges:
    return "TriggerMode::GateChanges";
  case TriggerMode::GateIsHigh:
    return "TriggerMode::GateIsHigh";
  case TriggerMode::GateIsLow:
    return "TriggerMode::GateIsLow";
  }
}
} // namespace curve_sequencer
} // namespace test
namespace dhe {
namespace curve_sequencer {

static inline auto operator<<(std::ostream &os, CompletionMode mode)
    -> std::ostream & {
  os << test::curve_sequencer::name_of(mode);
  return os;
}

static inline auto operator<<(std::ostream &os, InterruptMode mode)
    -> std::ostream & {
  os << test::curve_sequencer::name_of(mode);
  return os;
}

static inline auto operator<<(std::ostream &os, TriggerMode mode)
    -> std::ostream & {
  os << test::curve_sequencer::name_of(mode);
  return os;
}

} // namespace curve_sequencer
} // namespace dhe
