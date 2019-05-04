#include "components/RangedDuration.h"

#include "util/duration.h"
#include "util/range.h"

namespace DHE {

static constexpr float medium_duration_switch_position{1.f};
static rack::engine::Input default_cv_input{};
static rack::engine::Param default_switch_param{};

void RangedDuration::config(rack::engine::Param *knob_param) {
  default_switch_param.setValue(medium_duration_switch_position);

  config(knob_param, &default_switch_param, &default_cv_input);
}

void RangedDuration::config(rack::engine::Param *knob_param,
                            rack::engine::Param *switch_param,
                            rack::engine::Input *cv_input) {
  this->knob_param = knob_param;
  this->switch_param = switch_param;
}

auto RangedDuration::operator()() -> float {
  auto const range_index = static_cast<int>(switch_param->getValue());
  auto const *range = Duration::ranges()[range_index];
  auto const rotation = knob_param->getValue();
  auto tapered = Sigmoid::j_shape.taper(rotation, 0.802f);
  return range->scale(tapered);
}

} // namespace DHE
