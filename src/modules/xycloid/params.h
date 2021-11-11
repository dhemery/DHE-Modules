#pragma once

#include "components/range.h"
#include "components/sigmoid.h"
#include "controls/knobs.h"

#include "rack.hpp"

#include <vector>

namespace dhe {
namespace xycloid {

struct WobbleRatioModes {
  enum Selection { Quantized, Free };

  static inline auto labels() -> std::vector<std::string> const & {
    static auto const labels = std::vector<std::string>{"Quantized", "Free"};
    return labels;
  }
};

struct WobbleRatios {
  enum Selection { Inward, InwardOutward, Outward };
  static auto constexpr unit = "x";

  static inline auto labels() -> std::vector<std::string> const & {
    static auto const labels =
        std::vector<std::string>{"In", "-In +Out", "Out"};
    return labels;
  }

  static inline auto select(int selection) -> Range const & {
    static auto constexpr max_ratio = 16.F;
    static auto const ranges =
        std::vector<Range>{Range{0.F, -max_ratio}, Range{-max_ratio, max_ratio},
                           Range{0.F, max_ratio}};
    return ranges[selection];
  }

  static inline auto ratio(float rotation, int range, bool quantize) -> float {
    auto const ratio = select(range).scale(rotation);
    return quantize ? std::round(ratio) : ratio;
  }

  static inline auto rotation(float ratio, int range) -> float {
    return select(range).normalize(ratio);
  }

  struct KnobMapper {
    auto to_display_value(float rotation) const -> float {
      return ratio(rotation, range_, quantize_);
    }

    auto to_rotation(float ratio) const -> float {
      return rotation(ratio, range_);
    }

    void select_mode(WobbleRatioModes::Selection selection) {
      quantize_ = selection == WobbleRatioModes::Quantized;
    }

    void select_range(Selection selection) { range_ = selection; }

  private:
    bool quantize_{false};
    int range_{Outward};
  };
};

struct ThrobSpeed {
  static auto constexpr unit = " Hz";

  static auto constexpr hertz(float rotation) -> float {
    return range.scale(taper.apply(rotation));
  }

  static auto constexpr rotation(float speed) -> float {
    return taper.invert(range.normalize(speed));
  }

  struct KnobMapper {
    auto to_display_value(float rotation) const -> float {
      return hertz(rotation);
    }

    auto to_rotation(float ratio) const -> float { return rotation(ratio); }
  };

private:
  static auto constexpr range = Range{-10.F, 10.F};
  static auto constexpr taper = sigmoid::s_taper_with_curvature(-0.8F);
};
} // namespace xycloid
} // namespace dhe
