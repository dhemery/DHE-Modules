#pragma once

#include "engine/Param.hpp"
#include "engine/Port.hpp"

#include "util/duration.h"
#include "util/range.h"

namespace DHE {

class RangedDuration {
public:
  auto operator()() -> float {
    int range_index = static_cast<int>(switch_param->getValue());
    Range const *range = Duration::ranges()[range_index];
    return range->lower_bound;
  }
  void config(rack::engine::Param *knob_param,
              rack::engine::Param *switch_param,
              rack::engine::Input *cv_input) {
    this->switch_param = switch_param;
  }

private:
  rack::engine::Param *knob_param;
  rack::engine::Param *switch_param;
  rack::engine::Input *cv_input;
};

} // namespace DHE
