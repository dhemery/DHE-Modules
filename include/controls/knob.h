#pragma once

#include <utility>

#include <engine.hpp>

#include "util/range.h"

namespace DHE {
static constexpr auto constant_zero = 0.f;
static constexpr auto constant_one = 1.f;

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

class Knob {
public:
  Knob(const rack::Module *module, int knob_index)
      : Knob{&module->params[knob_index].value, &constant_zero, &constant_one} {
  }
  Knob(const rack::Module *module, int knob_index, int cv_index)
      : Knob{&module->params[knob_index].value, &module->inputs[cv_index].value,
             &constant_one} {}
  Knob(const rack::Module *module, int knob_index, int cv_index, int av_index)
      : Knob{&module->params[knob_index].value, &module->inputs[cv_index].value,
             &module->params[av_index].value} {}

  inline auto operator()() const -> float { return modulated(*knob, *cv, *av); }

private:
  explicit Knob(const float *knob, const float *cv, const float *av)
      : knob{knob}, cv{cv}, av{av} {}

  const float *knob;
  const float *cv;
  const float *av;
};

class ScaledKnob {
public:
  ScaledKnob(const rack::Module *module, int knob_index, int cv_index,
             Range range)
      : knob{&module->params[knob_index].value},
        cv{&module->inputs[cv_index].value}, range{range} {}

  inline auto operator()() const -> float {
    return range.scale(modulated(*knob, *cv));
  }

private:
  const float *knob;
  const float *cv;
  const Range range;
};
} // namespace DHE
