#include <array>

#include "modules/controls/Duration.h"
#include "util/sigmoid.h"

#include "util/gain.h"

namespace DHE {

constexpr auto av_range = Range{-1.f, 1.f};

inline auto cv_offset(float bipolar_voltage) -> float {
  static constexpr auto cv_to_offset = 0.1f;
  return bipolar_voltage * cv_to_offset;
}

inline auto av_multiplier(float av_amount) -> float {
  return av_range.scale(av_amount);
}

inline auto gain_multiplier(float gain_amount) -> float {
  return Gain::range.scale(gain_amount);
}

inline auto modulated(float knob_rotation, float cv_bipolar_voltage,
                      float av_amount) -> float {
  return knob_rotation +
      av_multiplier(av_amount) * cv_offset(cv_bipolar_voltage);
}

inline auto modulated(float knob_rotation, float cv_bipolar_voltage) -> float {
  return knob_rotation + cv_offset(cv_bipolar_voltage);
}

// Note that each range is of the form [n, 1000n].
const Range Duration::short_range{0.001f, 1.f};
const Range Duration::medium_range{0.01f, 10.f};
const Range Duration::long_range{0.1f, 100.f};

ConstantParam Duration::defaultRangeSwitch{1.f};

const std::array<Range const *, 3> Duration::ranges{&short_range, &medium_range,
                                                    &long_range};

auto Duration::seconds() -> float {
  /**
   * Each duration range is of the form [n, 1000n]. Given ranges of that form,
   * this curvature tapers the rotation so a knob positioned dead center yields
   * a duration equal to 1/10 of the range's upper bound (to within 7 decimal
   * places).
   */
  static auto constexpr curvature = 0.8018017;
  auto const rotation = knob.getValue();
  auto const cv_voltage = cvInput.getVoltage();
  auto const modulated_rotation = modulated(rotation, cv_voltage);
  auto const tapered = Sigmoid::j_shape.taper(modulated_rotation, curvature);

  auto const range_index = static_cast<int>(rangeSwitch.getValue());
  auto const *range = ranges[range_index];
  return range->scale(tapered);
}

} // namespace DHE

