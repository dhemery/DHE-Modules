#pragma once

#include <utility>

#include <engine.hpp>

#include "util/range.h"

namespace DHE {

auto default_cv() -> const rack::Input *;
auto default_av() -> const rack::Param *;

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
      : Knob{&module->params[knob_index]} {}
  Knob(const rack::Module *module, int knob_index, int cv_index)
      : Knob{&module->params[knob_index], &module->inputs[cv_index]} {}
  Knob(const rack::Module *module, int knob_index, int cv_index, int av_index)
      : Knob{&module->params[knob_index], &module->inputs[cv_index],
             &module->params[av_index]} {}

  virtual auto operator()() const -> float {
    return modulated(knob->value, cv->value, av->value);
  }

private:
  explicit Knob(const rack::Param *knob, const rack::Input *cv = default_cv(),
                const rack::Param *av = default_av())
      : knob{knob}, cv{cv}, av{av} {}

  const rack::Param *knob;
  const rack::Input *cv;
  const rack::Param *av;
};

class ScaledKnob {
public:
  ScaledKnob(const rack::Module *module, int knob_index, int cv_index,
             Range range)
      : knob{&module->params[knob_index]}, cv{&module->inputs[cv_index]},
        range{range} {}

  inline auto operator()() const -> float {
    return range.scale(modulated(knob->value, cv->value));
  }

private:
  const rack::Param *knob;
  const rack::Input *cv;
  const Range range;
};
} // namespace DHE
