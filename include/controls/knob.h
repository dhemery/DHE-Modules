#pragma once

#include "util/range.h"
#include <engine.hpp>

namespace DHE {
inline auto cv_offset(float cv) -> float {
  static constexpr auto cv_to_offset = 0.1f;
  return cv * cv_to_offset;
}

inline auto av_multiplier(float av) -> float {
  static constexpr auto av_range = Range{-1.f, 1.f};
  return av_range.scale(av);
}

inline auto modulated(float rotation, float cv, float av) -> float {
  return rotation + av_multiplier(av) * cv_offset(cv);
}

inline auto modulated(float rotation, float cv) -> float {
  return rotation + cv_offset(cv);
}

inline auto modulated(const rack::Module *module, int knob_param, int cv_input,
                      int av_param) -> float {
  auto knob = module->params[knob_param].value;
  auto cv = module->inputs[cv_input].value;
  auto av = module->params[av_param].value;
  return modulated(knob, cv, av);
}

inline auto modulated(const rack::Module *module, int knob_param, int cv_input)
    -> float {
  auto knob = module->params[knob_param].value;
  auto cv = module->inputs[cv_input].value;
  return modulated(knob, cv);
}
} // namespace DHE
