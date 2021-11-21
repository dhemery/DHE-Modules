#pragma once

#include "components/range.h"
#include "components/sigmoid.h"
#include "controls/knobs.h"
#include "signals/enums.h"

#include "rack.hpp"

#include <vector>

namespace dhe {
namespace xycloid {

enum class WobbleRatioMode { Quantized, Free };

struct WobbleRatioModes : Enums<WobbleRatioMode, 2> {
  static inline auto labels() -> std::vector<std::string> const & {
    static auto const labels = std::vector<std::string>{"Quantized", "Free"};
    return labels;
  }
};

enum class WobbleRatioRangeId { Inward, InwardOutward, Outward };

struct WobbleRatioRanges {
  static auto constexpr size = 3;
  using value_type = WobbleRatioRangeId;
  struct KnobMap;

  static inline auto labels() -> std::vector<std::string> const & {
    static auto const labels =
        std::vector<std::string>{"In", "-In +Out", "Out"};
    return labels;
  }
};

struct WobbleRatio {
  struct KnobMap;

  static inline auto scale(float normalized, WobbleRatioRangeId range_id,
                           WobbleRatioMode mode) -> float {
    auto const ratio = range(range_id).scale(normalized);
    return mode == WobbleRatioMode::Quantized ? std::round(ratio) : ratio;
  }

  static inline auto normalize(float scaled, WobbleRatioRangeId range_id)
      -> float {
    return range(range_id).normalize(scaled);
  }

  static inline auto range(WobbleRatioRangeId id) -> Range {
    return ranges()[static_cast<size_t>(id)];
  }

  static inline auto ranges() -> std::vector<Range> const & {
    static auto constexpr max_ratio = 16.F;
    static auto const ranges =
        std::vector<Range>{Range{0.F, -max_ratio}, Range{-max_ratio, max_ratio},
                           Range{0.F, max_ratio}};
    return ranges;
  }
};

struct WobbleRatio::KnobMap {
  static auto constexpr default_value = 8.F;
  static auto constexpr unit = "x";

  auto to_display(float value) const -> float {
    return scale(value, range_id_, mode_);
  }

  auto to_value(float display) const -> float {
    return normalize(display, range_id_);
  }

  void select_mode(WobbleRatioMode mode) { mode_ = mode; }

  void select_range(WobbleRatioRangeId id) { range_id_ = id; }

private:
  WobbleRatioMode mode_{WobbleRatioMode::Free};
  WobbleRatioRangeId range_id_{WobbleRatioRangeId::Outward};
};

} // namespace xycloid
} // namespace dhe
