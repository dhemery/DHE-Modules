#pragma once

#include <string>
#include <vector>

namespace dhe {

namespace sequencizer {

struct AnchorModes {
  using value_type = AnchorMode; // NOLINT
  static auto constexpr stepper_slug = "anchor-mode";

  static inline auto labels() -> std::vector<std::string> const & {
    static auto const labels =
        std::vector<std::string>{"Sample the source", "Track the source"};
    return labels;
  }
};

struct AnchorSources {
  using value_type = AnchorSource; // NOLINT
  static auto constexpr stepper_slug = "anchor-source";

  static inline auto labels() -> std::vector<std::string> const & {
    static auto const labels =
        std::vector<std::string>{"Level", "A", "B", "C", "Out"};
    return labels;
  }
};

struct InterruptModes {
  using value_type = InterruptMode; // NOLINT
  static auto constexpr stepper_slug = "interrupt-mode";

  static inline auto labels() -> std::vector<std::string> const & {
    static auto const labels = std::vector<std::string>{
        "Ignore triggers while generating", "Interrupt if triggered"};
    return labels;
  }
};

struct SustainModes {
  using value_type = SustainMode; // NOLINT
  static auto constexpr stepper_slug = "sustain-mode";

  static inline auto labels() -> std::vector<std::string> {
    static auto const labels =
        std::vector<std::string>{"No sustain", "Sustain until triggered"};
    return labels;
  }
};

struct TriggerModes {
  using value_type = TriggerMode; // NOLINT
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
