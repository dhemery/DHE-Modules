#pragma once

#include <utility>

#include <engine.hpp>

#include "util/range.h"

namespace DHE {

auto default_cv() -> const rack::Input *;
auto default_av() -> const rack::Param *;

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
    static constexpr auto av_range = Range{-1.f, 1.f};
    static constexpr auto cv_to_offset = 0.1f;
    auto rotation = knob->value;
    auto offset = cv->value * cv_to_offset;
    auto multiplier = av_range.scale(av);
    return rotation + multiplier * offset;
  }

private:
  Knob(const rack::Param *knob, const rack::Input *cv = default_cv(),
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

  auto operator()() const -> float {
    auto rotation = knob->value;
    auto offset = cv->value * 0.1f;
    return range.scale(rotation + offset);
  }

private:
  const rack::Param *knob;
  const rack::Input *cv;
  const Range range;
};
} // namespace DHE
