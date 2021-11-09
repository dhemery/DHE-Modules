#pragma once

#include <string>
#include <vector>

namespace dhe {

namespace sequencizer {

struct AnchorModes {
  using PositionType = AnchorMode;
  static auto constexpr stepper_slug = "anchor-mode";

  static inline auto labels() -> std::vector<std::string> const & {
    static auto const labels =
        std::vector<std::string>{"Sample the source", "Track the source"};
    return labels;
  }
};

struct AnchorSources {
  using PositionType = AnchorSource;
  static auto constexpr stepper_slug = "anchor-source";

  static inline auto labels() -> std::vector<std::string> const & {
    static auto const labels =
        std::vector<std::string>{"Level", "A", "B", "C", "Out"};
    return labels;
  }
};

struct InterruptModes {
  using PositionType = InterruptMode;
  static auto constexpr stepper_slug = "interrupt-mode";

  static inline auto labels() -> std::vector<std::string> const & {
    static auto const labels = std::vector<std::string>{
        "Ignore triggers while generating", "Interrupt if triggered"};
    return labels;
  }
};

struct SustainModes {
  using PositionType = SustainMode;
  static auto constexpr stepper_slug = "sustain-mode";

  static inline auto labels() -> std::vector<std::string> {
    static auto const labels =
        std::vector<std::string>{"No sustain", "Sustain until triggered"};
    return labels;
  }
};

struct TriggerModes {
  using PositionType = TriggerMode;
  static auto constexpr stepper_slug = "trigger-mode";

  static inline auto labels() -> std::vector<std::string> {
    static auto const labels = std::vector<std::string>{
        "Gate rises", "Gate falls", "Gate rises or falls", "Gate is high",
        "Gate is low"};
    return labels;
  }
};

} // namespace sequencizer
} // namespace dhe
