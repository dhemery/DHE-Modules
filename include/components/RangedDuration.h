#pragma once

#include "engine/Param.hpp"
#include "engine/Port.hpp"

namespace DHE {

class RangedDuration {
public:
  void config(rack::engine::Param *knob_param,
              rack::engine::Param *switch_param,
              rack::engine::Input *cv_input);
  auto operator()() -> float;

private:
  rack::engine::Param *knob_param;
  rack::engine::Param *switch_param;
  rack::engine::Input *cv_input;
};

} // namespace DHE
