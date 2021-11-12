#pragma once

#include "components/range.h"
#include "components/sigmoid.h"
#include "controls/knobs.h"
#include "params/presets.h"

#include "rack.hpp"

#include <string>
#include <vector>

namespace dhe {
namespace blossom {

enum class BounceRatioMode { Quantized, Free };

struct BounceRatioModes {
  using Selection = BounceRatioMode;

  static auto labels() -> std::vector<std::string> const & {
    static auto const labels = std::vector<std::string>{"Quantized", "Free"};
    return labels;
  }
};

struct BounceRatio {
  struct KnobMapper;
  static auto constexpr unit = "x";

  static inline auto ratio(float rotation, BounceRatioMode mode) -> float {
    auto const ratio = range().scale(rotation);
    return mode == BounceRatioMode::Quantized ? std::round(ratio) : ratio;
  }

  static inline auto rotation(float ratio) -> float {
    return range().normalize(ratio);
  }

private:
  static inline auto range() -> Range { return Range{1.F, 17.F}; }
};

struct BounceRatio::KnobMapper {
  auto to_display_value(float rotation) const -> float {
    return ratio(rotation, mode_);
  }

  auto to_rotation(float ratio) const -> float { return rotation(ratio); }

  void select_mode(BounceRatioMode mode) { mode_ = mode; }

private:
  BounceRatioMode mode_{BounceRatioMode::Free};
};

struct SpinSpeed {
  static auto constexpr unit = " Hz";

  static auto rotation(float hertz) -> float {
    return taper().invert(range().normalize(hertz));
  }

  static auto hertz(float rotation) -> float {
    return range().scale(taper().apply(rotation));
  }

  struct KnobMapper {
    auto to_display_value(float rotation) const -> float {
      return hertz(rotation);
    }

    auto to_rotation(float hertz) const -> float { return rotation(hertz); }
  };

private:
  static auto constexpr range() -> Range { return Range{-10.F, 10.F}; }
  static auto taper() -> sigmoid::Taper const & {
    static auto const taper = sigmoid::s_taper_with_curvature(-0.8F);
    return taper;
  }
};

} // namespace blossom
} // namespace dhe
