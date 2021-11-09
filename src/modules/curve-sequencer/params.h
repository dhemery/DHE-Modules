#pragma once

#include "advance-mode.h"
#include "generate-mode.h"

namespace dhe {
namespace curve_sequencer {
struct AdvanceModes {
  using PositionType = AdvanceMode;
  static constexpr auto stepper_slug = "advance-mode";
  static inline auto labels() -> std::vector<std::string> const & {
    static auto const labels = std::vector<std::string>{
        "Timer expires", "Gate rises",   "Gate falls",
        "Gate changes",  "Gate is high", "Gate is low"};
    return labels;
  }
};

struct GenerateModes {
  using PositionType = GenerateMode;
  static constexpr auto stepper_slug = "generate-mode";
  static inline auto labels() -> std::vector<std::string> const & {
    static auto const labels = std::vector<std::string>{
        "Curve", "Hold", "Sustain", "InPort", "Chase", "Level"};
    return labels;
  }
};

} // namespace curve_sequencer
} // namespace dhe
