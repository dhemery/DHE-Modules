#pragma once

#include "components/range.h"
#include "components/sigmoid.h"
#include "controls/knobs.h"

#include <array>

namespace dhe {
namespace xycloid {
namespace wobble_ratio {

namespace mode {
static auto constexpr size = 2;
static auto constexpr labels =
    std::array<char const *, size>{"Quantized", "Free"};
} // namespace mode

namespace range {
static auto constexpr size = 3;
static auto constexpr labels =
    std::array<char const *, size>{"In", "-In +Out", "Out"};
} // namespace range
} // namespace wobble_ratio

enum class WobbleRatioMode { Quantized, Free };

struct WobbleRatioModes {
  using ValueType = WobbleRatioMode;
  static auto constexpr &labels = wobble_ratio::mode::labels;
  static auto constexpr size = wobble_ratio::mode::size;
};

enum class WobbleRatioRangeId { Inward, InwardOutward, Outward };

struct WobbleRatioRanges {
  using ValueType = WobbleRatioRangeId;
  static auto constexpr size = wobble_ratio::range::size;
  static auto constexpr &labels = wobble_ratio::range::labels;
};

struct WobbleRatio {
  struct KnobMap;

  static inline auto scale(float normalized, WobbleRatioRangeId range_id,
                           WobbleRatioMode mode) -> float {
    auto const ratio = range(range_id).scale(normalized);
    return mode == WobbleRatioMode::Quantized ? std::roundf(ratio) : ratio;
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
