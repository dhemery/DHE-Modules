#pragma once

#include <vector>
#include <string>

namespace dhe {
namespace curve_sequencer {
enum class GenerateMode { Curve, Hold, Sustain, Input, Chase, Level };

struct GenerateModes {
  using value_type = GenerateMode;
  static constexpr auto size = 6;
  static constexpr auto stepper_slug = "generate-mode";
  static inline auto labels() -> std::vector<std::string> const & {
    static auto const labels = std::vector<std::string>{
        "Curve", "Hold", "Sustain", "InPort", "Chase", "Level"};
    return labels;
  }
};

} // namespace curve_sequencer
} // namespace dhe
