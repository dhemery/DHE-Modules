#include <array>

#include "modules/controls/Duration.h"
#include "util/sigmoid.h"

namespace DHE {

// Note that each range is of the form [n, 1000n].
const Range Duration::short_range{0.001f, 1.f};
const Range Duration::medium_range{0.01f, 10.f};
const Range Duration::long_range{0.1f, 100.f};

const std::array<Range const *, 3> Duration::ranges{
    &short_range, &medium_range, &long_range
};

static constexpr float medium_duration_switch_position{1.f};

static rack::engine::Input default_cv_input{};
static rack::engine::Param default_switch_param{};

void Duration::config(rack::engine::Param *knob_param) {
  default_switch_param.setValue(medium_duration_switch_position);

  config(knob_param, &default_switch_param, &default_cv_input);
}

void Duration::config(rack::engine::Param *knob_param,
                      rack::engine::Param *switch_param,
                      rack::engine::Input *cv_input) {
  this->knob_param = knob_param;
  this->switch_param = switch_param;
}

auto Duration::operator()() -> float {
  /**
   * Each duration range is of the form [n, 1000n]. Given ranges of that form,
   * this curvature tapers the rotation so a knob positioned dead center yields
   * a duration equal to 1/10 of the range's upper bound (to within 7 decimal
   * places).
   */
  auto constexpr curvature = 0.8018017;

  auto const range_index = static_cast<int>(switch_param->getValue());
  auto const *range = ranges[range_index];
  auto const rotation = knob_param->getValue();
  auto const tapered = Sigmoid::j_shape.taper(rotation, curvature);
  return range->scale(tapered);
}

} // namespace DHE
