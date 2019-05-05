#include <array>

#include "modules/controls/Level.h"
#include "util/range.h"

namespace DHE {

const Range Level::unipolar_range{0.f, 10.f};
const Range Level::bipolar_range{-5.f, 5.f};

const std::array<Range const *, 2> Level::ranges{&bipolar_range,
                                                 &unipolar_range};

static constexpr float medium_duration_switch_position{1.f};

static rack::engine::Input default_cv_input{};
static rack::engine::Param default_switch_param{};

void Level::config(rack::engine::Param *knob_param) {
  default_switch_param.setValue(medium_duration_switch_position);

  config(knob_param, &default_switch_param, &default_cv_input);
}

void Level::config(rack::engine::Param *knob_param,
                   rack::engine::Param *switch_param) {
  default_switch_param.setValue(medium_duration_switch_position);

  config(knob_param, switch_param, &default_cv_input);
}

void Level::config(rack::engine::Param *knob_param,
                   rack::engine::Param *switch_param,
                   rack::engine::Input *cv_input) {
  this->knob_param = knob_param;
  this->switch_param = switch_param;
}

auto Level::operator()() -> float {
  auto const range_index = static_cast<int>(switch_param->getValue());
  auto const *range = ranges[range_index];
  auto const rotation = knob_param->getValue();
  return range->scale(rotation);
}

} // namespace DHE
