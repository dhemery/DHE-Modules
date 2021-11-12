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

enum class WobbleRatioRange { Inward, InwardOutward, Outward };

struct WobbleRatioRanges {
  struct KnobMapper;
  using Selection = WobbleRatioRange;
  static auto constexpr unit = "x";

  static inline auto labels() -> std::vector<std::string> const & {
    static auto const labels =
        std::vector<std::string>{"In", "-In +Out", "Out"};
    return labels;
  }

  static inline auto ratio(float rotation, WobbleRatioRange range,
                           WobbleRatioMode mode) -> float {
    auto const ratio = select_range(range).scale(rotation);
    return mode == WobbleRatioMode::Quantized ? std::round(ratio) : ratio;
  }

  static inline auto rotation(float ratio, WobbleRatioRange range) -> float {
    return select_range(range).normalize(ratio);
  }

private:
  static inline auto select_range(WobbleRatioRange range) -> Range const & {
    static auto constexpr max_ratio = 16.F;
    static auto const ranges =
        std::vector<Range>{Range{0.F, -max_ratio}, Range{-max_ratio, max_ratio},
                           Range{0.F, max_ratio}};
    return ranges[(int)range];
  }
};

struct WobbleRatioRanges::KnobMapper {
  auto to_display_value(float rotation) const -> float {
    return ratio(rotation, range_, mode_);
  }

  auto to_rotation(float ratio) const -> float {
    return rotation(ratio, range_);
  }

  void select_mode(WobbleRatioMode mode) { mode_ = mode; }

  void select_range(Selection selection) { range_ = selection; }

private:
  WobbleRatioMode mode_{WobbleRatioMode::Free};
  WobbleRatioRange range_{WobbleRatioRange::Outward};
};

struct ThrobSpeed {
  struct KnobMapper;
  static auto constexpr unit = " Hz";

  static auto hertz(float rotation) -> float {
    return range().scale(taper().apply(rotation));
  }

  static auto rotation(float speed) -> float {
    return taper().invert(range().normalize(speed));
  }

private:
  static auto constexpr range() -> Range { return Range{-10.F, 10.F}; }
  static auto taper() -> sigmoid::Taper const & {
    static auto const taper = sigmoid::s_taper_with_curvature(-0.8F);
    return taper;
  }
};

struct ThrobSpeed::KnobMapper {
  auto to_display_value(float rotation) const -> float {
    return hertz(rotation);
  }

  auto to_rotation(float ratio) const -> float { return rotation(ratio); }
};

} // namespace xycloid
} // namespace dhe
