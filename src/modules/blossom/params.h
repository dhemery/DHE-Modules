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
  using value_type = BounceRatioMode; // NOLINT

  static auto labels() -> std::vector<std::string> const & {
    static auto const labels = std::vector<std::string>{"Quantized", "Free"};
    return labels;
  }
};

struct BounceRatio {
  struct KnobMapper;
  static auto constexpr default_rotation = 0.F;
  static auto constexpr unit = "x";

  static inline auto scale(float normalized, BounceRatioMode mode) -> float {
    auto const ratio = range().scale(normalized);
    return mode == BounceRatioMode::Quantized ? std::round(ratio) : ratio;
  }

  static inline auto normalize(float scaled) -> float {
    return range().normalize(scaled);
  }

private:
  static inline auto range() -> Range { return Range{1.F, 17.F}; }
};

struct BounceRatio::KnobMapper {
  auto scale(float normalized) const -> float {
    return BounceRatio::scale(normalized, mode_);
  }

  auto normalize(float scaled) const -> float {
    return BounceRatio::normalize(scaled);
  }

  void select_mode(BounceRatioMode mode) { mode_ = mode; }

private:
  BounceRatioMode mode_{BounceRatioMode::Free};
};

struct SpinSpeed {
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

struct SpinSpeed::KnobMapper {
  auto scale(float normalized) const -> float {
    return SpinSpeed::scale(normalized);
  }

  auto normalize(float scaled) const -> float {
    return SpinSpeed::normalize(scaled);
  }
};

} // namespace blossom
} // namespace dhe
