#pragma once

#include "advance-mode.h"
#include "generate-mode.h"
#include "signals/enums.h"

namespace dhe {
namespace curve_sequencer {
struct AdvanceModes : Enums<AdvanceMode, 6> {
  static constexpr auto stepper_slug = "advance-mode";
  static inline auto labels() -> std::vector<std::string> const & {
    static auto const labels = std::vector<std::string>{
        "Timer expires", "Gate rises",   "Gate falls",
        "Gate changes",  "Gate is high", "Gate is low"};
    return labels;
  }
};

struct GenerateModes : Enums<GenerateMode, 6> {
  using value_type = GenerateMode; // NOLINT
  static constexpr auto stepper_slug = "generate-mode";
  static inline auto labels() -> std::vector<std::string> const & {
    static auto const labels = std::vector<std::string>{
        "Curve", "Hold", "Sustain", "InPort", "Chase", "Level"};
    return labels;
  }
};

} // namespace curve_sequencer
} // namespace dhe
