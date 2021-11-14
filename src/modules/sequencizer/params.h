#pragma once

#include "signals/enums.h"

#include <string>
#include <vector>

namespace dhe {

namespace sequencizer {

struct AnchorModes : Enums<AnchorMode, 2> {
  static auto constexpr stepper_slug = "anchor-mode";

  static inline auto labels() -> std::vector<std::string> const & {
    static auto const labels =
        std::vector<std::string>{"Sample the source", "Track the source"};
    return labels;
  }
};

struct AnchorSources : Enums<AnchorSource, 5> {
  static auto constexpr stepper_slug = "anchor-source";

  static inline auto labels() -> std::vector<std::string> const & {
    static auto const labels =
        std::vector<std::string>{"Level", "A", "B", "C", "Out"};
    return labels;
  }
};

struct InterruptModes : Enums<InterruptMode, 2> {
  static auto constexpr stepper_slug = "interrupt-mode";

  static inline auto labels() -> std::vector<std::string> const & {
    static auto const labels = std::vector<std::string>{
        "Ignore triggers while generating", "Interrupt if triggered"};
    return labels;
  }
};

struct SustainModes : Enums<SustainMode, 2> {
  static auto constexpr stepper_slug = "sustain-mode";

  static inline auto labels() -> std::vector<std::string> const & {
    static auto const labels =
        std::vector<std::string>{"No sustain", "Sustain until triggered"};
    return labels;
  }
};

struct TriggerModes : Enums<TriggerMode, 5> {
  static auto constexpr stepper_slug = "trigger-mode";

  static inline auto labels() -> std::vector<std::string> const & {
    static auto const labels = std::vector<std::string>{
        "Gate rises", "Gate falls", "Gate rises or falls", "Gate is high",
        "Gate is low"};
    return labels;
  }
};

} // namespace sequencizer
} // namespace dhe
