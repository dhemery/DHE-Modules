#pragma once

#include "advance-mode.h"
#include "generate-mode.h"

namespace dhe {
namespace curve_sequencer {
struct AdvanceModes {
  using ValueType = AdvanceMode;
  static constexpr auto frame_prefix = "advance-mode";
  static inline auto labels() -> std::vector<std::string> const & {
    static auto const labels = std::vector<std::string>{
        "Timer expires", "Gate rises",   "Gate falls",
        "Gate changes",  "Gate is high", "Gate is low"};
    return labels;
  }
};

struct GenerateModes {
  using ValueType = GenerateMode;
  static constexpr auto frame_prefix = "generate-mode";
  static inline auto labels() -> std::vector<std::string> const & {
    static auto const labels = std::vector<std::string>{
        "Curve", "Hold", "Sustain", "InPort", "Chase", "Level"};
    return labels;
  }
};

} // namespace curve_sequencer
} // namespace dhe
