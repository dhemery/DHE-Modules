#pragma once

#include "components/cxmath.h"
#include "components/range.h"

namespace dhe {
template <typename T> struct LinearSignal {
  static constexpr auto scale(float normalized) -> float {
    return T::range.scale(normalized);
  }

  static constexpr auto normalize(float scaled) -> float {
    return T::range.normalize(scaled);
  }

  static constexpr auto clamp(float unsafe) -> float {
    return T::range.clamp(unsafe);
  }
};

namespace linear {
static auto constexpr attenuverter_range = Range{-1.F, 1.F};
static auto constexpr gain_range = Range{0.F, 2.F};
static auto constexpr phase_range = Range{-pi, pi};
static auto constexpr rotation_range = Range{0.F, 1.F};
} // namespace linear

struct Attenuverter : LinearSignal<Attenuverter> {
  static auto constexpr &range = linear::attenuverter_range;
};

struct Gain : LinearSignal<Gain> {
  static auto constexpr &range = linear::gain_range;
};

struct Phase : LinearSignal<Phase> {
  static auto constexpr &range = linear::phase_range;
};

struct Rotation : LinearSignal<Rotation> {
  static auto constexpr &range = linear::rotation_range;
};

using Attenuator = Rotation;
} // namespace dhe
