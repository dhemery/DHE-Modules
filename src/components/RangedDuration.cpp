#include "components/RangedDuration.h"

#include "util/duration.h"
#include "util/range.h"

namespace DHE {

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
  auto const duration = range->scale(rotation);
  return duration;
}

} // namespace DHE
