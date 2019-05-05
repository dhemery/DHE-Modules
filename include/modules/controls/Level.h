#pragma once

#include <array>

#include "engine/Param.hpp"
#include "engine/Port.hpp"

#include "util/range.h"

namespace DHE {

class Level {
public:
  void config(rack::engine::Param *knob_param);
  void config(rack::engine::Param *knob_param,
              rack::engine::Param *switch_param);
  void config(rack::engine::Param *knob_param,
              rack::engine::Param *switch_param,
              rack::engine::Input *cv_input);

  auto operator()() -> float;

  static const Range unipolar_range;
  static const Range bipolar_range;
  static const std::array<Range const *, 2> ranges;

private:
  rack::engine::Param *knob_param;
  rack::engine::Param *switch_param;
  rack::engine::Input *cv_input;
};

} // namespace DHE
