#pragma once

#include "components/latch.h"

#include <array>

namespace dhe {
namespace truth {
enum class GateMode {
  Rise,
  Fall,
  Edge,
  High,
  Low,
};

namespace gate_mode {
static auto constexpr size = 5;
static auto constexpr labels = std::array<char const *, size>{
    "Gate rises", "Gate falls", "Gate rises or falls", "Gate is high",
    "Gate is low"};
} // namespace gate_mode

struct GateModes {
  using ValueType = GateMode;
  static auto constexpr &labels = gate_mode::labels;
  static auto constexpr size = gate_mode::size;
  static auto constexpr slug = "gate-mode";
};

static inline auto is_satisfied(GateMode condition, dhe::Latch const &gate)
    -> bool {
  switch (condition) {
  case GateMode::High:
  default:
    return gate.is_high();
  case GateMode::Rise:
    return gate.is_rise();
  case GateMode::Fall:
    return gate.is_fall();
  case GateMode::Edge:
    return gate.is_edge();
  case GateMode::Low:
    return gate.is_low();
  }
}

} // namespace truth
} // namespace dhe
