#pragma once

#include <array>
#include <ostream>
#include <string>

namespace dhe {
namespace curve_sequencer {

enum class AdvanceMode {
  TimerExpires,
  GateRises,
  GateFalls,
  GateChanges,
  GateIsHigh,
  GateIsLow
};

namespace advance_mode {
static auto constexpr size = 6;
static auto constexpr type_name = "AdvanceMode";
static auto constexpr values = std::array<AdvanceMode, size>{
    AdvanceMode::TimerExpires, AdvanceMode::GateRises,  AdvanceMode::GateFalls,
    AdvanceMode::GateChanges,  AdvanceMode::GateIsHigh, AdvanceMode::GateIsLow,
};

static auto constexpr labels = std::array<char const *, size>{
    "Timer expires", "Gate rises",   "Gate falls",
    "Gate changes",  "Gate is high", "Gate is low"};

static auto constexpr names =
    std::array<char const *, size>{"TimerExpires", "GateRises",  "GateFalls",
                                   "GateChanges",  "GateIsHigh", "GateIsHigh"};

static inline auto name(AdvanceMode mode) -> std::string {
  return names[static_cast<size_t>(mode)];
}
} // namespace advance_mode

struct AdvanceModes {
  using ValueType = AdvanceMode;
  static auto constexpr size = advance_mode::size;
  static auto constexpr stepper_slug = "advance-mode";
  static auto constexpr &labels = advance_mode::labels;
};

static inline auto operator<<(std::ostream &os, AdvanceMode mode)
    -> std::ostream & {
  auto const index = static_cast<size_t>(mode);
  if (index >= advance_mode::size) {
    return os << "Unknown " << advance_mode::type_name << ' ' << index;
  }
  return os << advance_mode::name(mode);
}

} // namespace curve_sequencer
} // namespace dhe
