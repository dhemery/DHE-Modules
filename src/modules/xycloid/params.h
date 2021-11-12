#pragma once

#include "components/range.h"
#include "components/sigmoid.h"
#include "controls/knobs.h"

#include "rack.hpp"

#include <vector>

namespace dhe {
namespace xycloid {

enum class WobbleRatioMode { Quantized, Free };

struct WobbleRatioModes {
  using Selection = WobbleRatioMode;

  static inline auto labels() -> std::vector<std::string> const & {
    static auto const labels = std::vector<std::string>{"Quantized", "Free"};
    return labels;
  }
};

enum class WobbleRatioRangeId { Inward, InwardOutward, Outward };

struct WobbleRatioRanges {
  struct KnobMapper;
  using Selection = WobbleRatioRangeId;
  static auto constexpr unit = "x";

  static inline auto labels() -> std::vector<std::string> const & {
    static auto const labels =
        std::vector<std::string>{"In", "-In +Out", "Out"};
    return labels;
  }

  static inline auto scale(float normalized, WobbleRatioRangeId range_id,
                           WobbleRatioMode mode) -> float {
    auto const ratio = range(range_id).scale(normalized);
    return mode == WobbleRatioMode::Quantized ? std::round(ratio) : ratio;
  }

  static inline auto normalize(float scaled, WobbleRatioRangeId range_id)
      -> float {
    return range(range_id).normalize(scaled);
  }

private:
  static inline auto range(WobbleRatioRangeId id) -> Range const & {
    static auto constexpr max_ratio = 16.F;
    static auto const ranges =
        std::vector<Range>{Range{0.F, -max_ratio}, Range{-max_ratio, max_ratio},
                           Range{0.F, max_ratio}};
    return ranges[(int)id];
  }
};

struct WobbleRatioRanges::KnobMapper {
  auto scale(float normalized) const -> float {
    return WobbleRatioRanges::scale(normalized, range_id_, mode_);
  }

  auto normalize(float scaled) const -> float {
    return WobbleRatioRanges::normalize(scaled, range_id_);
  }

  void select_mode(WobbleRatioMode mode) { mode_ = mode; }

  void select_range(WobbleRatioRangeId id) { range_id_ = id; }

private:
  WobbleRatioMode mode_{WobbleRatioMode::Free};
  WobbleRatioRangeId range_id_{WobbleRatioRangeId::Outward};
};

struct ThrobSpeed {
  struct KnobMapper;
  static auto constexpr unit = " Hz";

  static auto scale(float normalized) -> float {
    return range().scale(taper().apply(normalized));
  }

  static auto normalize(float scaled) -> float {
    return taper().invert(range().normalize(scaled));
  }

private:
  static auto constexpr range() -> Range { return Range{-10.F, 10.F}; }

  static auto taper() -> sigmoid::Taper const & {
    static auto const taper = sigmoid::s_taper_with_curvature(-0.8F);
    return taper;
  }
};

struct ThrobSpeed::KnobMapper {
  auto scale(float normalized) const -> float {
    return ThrobSpeed::scale(normalized);
  }

  auto normalize(float scaled) const -> float {
    return ThrobSpeed::normalize(scaled);
  }
};

} // namespace xycloid
} // namespace dhe
