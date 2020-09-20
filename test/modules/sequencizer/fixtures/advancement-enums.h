#pragma once

#include "modules/sequencizer/advancement.h"
#include <array>
#include <ostream>
#include <string>

namespace test {
namespace sequencizer {
using dhe::sequencizer::InterruptMode;
using dhe::sequencizer::SustainMode;
using dhe::sequencizer::TriggerMode;

static auto constexpr sustain_modes = std::array<SustainMode, 2>{
    SustainMode::Advance,
    SustainMode::Sustain,
};

static inline auto name_of(SustainMode mode) -> std::string {
  switch (mode) {
  case SustainMode::Advance:
    return "SustainMode::Advance";
  case SustainMode::Sustain:
    return "SustainMode::Sustain";
  default:
    return std::string{"Unknown SustainMode "} +
           std::to_string(static_cast<int>(mode));
  }
}

static auto constexpr interrupt_modes = std::array<InterruptMode, 2>{
    InterruptMode::Ignore,
    InterruptMode::Advance,
};

static inline auto name_of(InterruptMode mode) -> std::string {
  switch (mode) {
  case InterruptMode::Advance:
    return "InterruptMode::Advance";
  case InterruptMode::Ignore:
    return "InterruptMode::Ignore";
  default:
    return std::string{"Unknown InterruptMode "} +
           std::to_string(static_cast<int>(mode));
  }
}

static auto constexpr trigger_modes =
    std::array<TriggerMode, dhe::sequencizer::trigger_mode_count>{
        TriggerMode::GateRises,   TriggerMode::GateFalls,
        TriggerMode::GateChanges, TriggerMode::GateIsHigh,
        TriggerMode::GateIsLow,
    };

static inline auto name_of(dhe::sequencizer::TriggerMode mode) -> std::string {
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
  default:
    return std::string{"Unknown TriggerMode "} +
           std::to_string(static_cast<int>(mode));
  }
}
} // namespace sequencizer
} // namespace test
namespace dhe {
namespace sequencizer {

static inline auto operator<<(std::ostream &os, SustainMode mode)
    -> std::ostream & {
  os << test::sequencizer::name_of(mode);
  return os;
}

static inline auto operator<<(std::ostream &os, InterruptMode mode)
    -> std::ostream & {
  os << test::sequencizer::name_of(mode);
  return os;
}

static inline auto operator<<(std::ostream &os, TriggerMode mode)
    -> std::ostream & {
  os << test::sequencizer::name_of(mode);
  return os;
}

} // namespace sequencizer
} // namespace dhe
