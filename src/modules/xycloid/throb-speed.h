#pragma once

#include "components/range.h"
#include "components/sigmoid.h"
#include "controls/knobs.h"

namespace dhe {
namespace xycloid {
struct ThrobSpeed {
  struct KnobMap;

  static constexpr auto scale(float normalized) -> float {
    return cx::scale(SShape::apply(normalized, knob_taper), min_hz, max_hz);
  }

  static constexpr auto normalize(float scaled) -> float {
    return SShape::invert(cx::normalize(scaled, min_hz, max_hz), knob_taper);
  }

private:
  static auto constexpr knob_taper = -0.8F;
  static auto constexpr min_hz = -10.F;
  static auto constexpr max_hz = 10.F;
};

struct ThrobSpeed::KnobMap {
  static auto constexpr unit = " Hz";

  static constexpr auto to_display(float value) -> float {
    return scale(value);
  }

  static constexpr auto to_value(float display) -> float {
    return normalize(display);
  }
};

} // namespace xycloid
} // namespace dhe
