#pragma once

#include <string>
#include <vector>

namespace dhe {

namespace sequencizer {

struct AnchorModes {
  using TValue = AnchorMode;
  static auto constexpr frame_prefix = "anchor-mode";

  static inline auto labels() -> std::vector<std::string> const & {
    static auto const labels =
        std::vector<std::string>{"Sample the source", "Track the source"};
    return labels;
  }
};

struct AnchorSources {
  using TValue = AnchorSource;
  static auto constexpr frame_prefix = "anchor-source";

  static inline auto labels() -> std::vector<std::string> const & {
    static auto const labels =
        std::vector<std::string>{"Level", "A", "B", "C", "Out"};
    return labels;
  }
};

struct InterruptModes {
  using TValue = InterruptMode;
  static auto constexpr frame_prefix = "interrupt-mode";

  static inline auto labels() -> std::vector<std::string> const & {
    static auto const labels = std::vector<std::string>{
        "Ignore triggers while generating", "Interrupt if triggered"};
    return labels;
  }
};

struct Shapes {
  using TValue = int;
  static auto constexpr frame_prefix = "shape";

  static inline auto labels() -> std::vector<std::string> const & {
    static auto const labels = std::vector<std::string>{"J", "S"};
    return labels;
  }
};

struct SustainModes {
  using TValue = SustainMode;
  static auto constexpr frame_prefix = "sustain-mode";

  static inline auto labels() -> std::vector<std::string> {
    static auto const labels =
        std::vector<std::string>{"No sustain", "Sustain until triggered"};
    return labels;
  }
};

struct TriggerModes {
  using TValue = TriggerMode;
  static auto constexpr frame_prefix = "trigger-mode";

  static inline auto labels() -> std::vector<std::string> {
    static auto const labels = std::vector<std::string>{
        "Gate rises", "Gate falls", "Gate rises or falls", "Gate is high",
        "Gate is low"};
    return labels;
  }
};

} // namespace sequencizer
} // namespace dhe
