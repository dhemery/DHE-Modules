#pragma once

#include "components/range.h"
#include "components/sigmoid.h"
#include "controls/knobs.h"
#include "params/presets.h"
#include "signals/enums.h"

#include "rack.hpp"

#include <string>
#include <vector>

namespace dhe {
namespace blossom {

enum class BounceRatioMode { Quantized, Free };

struct BounceRatioModes : Enums<BounceRatioMode, 2> {
  static auto labels() -> std::vector<std::string> const & {
    static auto const labels = std::vector<std::string>{"Quantized", "Free"};
    return labels;
  }
};

struct BounceRatio {
  struct KnobMap;

  static inline auto scale(float normalized, BounceRatioMode mode) -> float {
    auto const ratio = range().scale(normalized);
    return mode == BounceRatioMode::Quantized ? std::round(ratio) : ratio;
  }

  static inline auto normalize(float scaled) -> float {
    return range().normalize(scaled);
  }

  static inline auto range() -> Range {
    static auto const range = Range{1.F, 17.F};
    return range;
  }
};

struct BounceRatio::KnobMap {
  static auto constexpr default_value = 9.F;
  static auto constexpr unit = "x";

  auto to_display(float value) const -> float { return scale(value, mode_); }

  static inline auto to_value(float display) -> float {
    return normalize(display);
  }

  void select_mode(BounceRatioMode mode) { mode_ = mode; }

private:
  BounceRatioMode mode_{BounceRatioMode::Free};
};

struct SpinSpeed {
  struct KnobMap;

  static inline auto scale(float normalized) -> float {
    return range().scale(SShape::apply(normalized, -0.8F));
  }

  static inline auto normalize(float scaled) -> float {
    return SShape::invert(range().normalize(scaled), -0.8F);
  }

  static inline auto range() -> Range {
    static auto const range = Range{-10.F, 10.F};
    return range;
  }
};

struct SpinSpeed::KnobMap {
  static auto constexpr default_value = 1.F;
  static auto constexpr unit = " Hz";

  static inline auto to_display(float value) -> float { return scale(value); }

  static inline auto to_value(float display) -> float {
    return normalize(display);
  }
};

} // namespace blossom
} // namespace dhe
