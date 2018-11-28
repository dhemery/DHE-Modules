#include <utility>

#include <utility>

#include <utility>

#include <utility>

#pragma once

#include <utility>

#include <engine.hpp>

#include "util/range.h"

namespace DHE {

class Knob {
public:
  Knob(const rack::Module *module, int knob_index)
      : Knob{module, &module->params[knob_index]} {}
  Knob(const rack::Module *module, int knob_index, int cv_index)
      : Knob{module, &module->params[knob_index], &module->inputs[cv_index]} {}
  Knob(const rack::Module *module, int knob_index, int cv_index, int av_index)
      : Knob{module, &module->params[knob_index], &module->inputs[cv_index], &module->params[av_index]} {}

  virtual auto operator()() const -> float {
    static constexpr auto av_range = Range{-1.f, 1.f};
    static constexpr auto cv_to_offset = 0.1f;
    auto rotation = knob->value;
    auto offset = cv->value*cv_to_offset;
    auto multiplier = av_range.scale(av);
    return function(rotation + multiplier*offset);
  }

  auto map(const std::function<float(float)> &new_function) const -> Knob;

  auto scale_to(Range range) const -> Knob;

  auto scale_to(const std::function<Range()> &range_supplier) const -> Knob;

private:
  static auto default_cv() -> const rack::Input *;
  static auto default_av() -> const rack::Param *;
  static auto identity(float input) -> float;

  Knob(const rack::Module *module, const rack::Param *knob,
       const rack::Input *cv = default_cv(), const rack::Param *av = default_av(),
       std::function<float(float)> function = &identity)
      : module{module}, knob{knob}, cv{cv}, av{av}, function{std::move(function)} {}

  const rack::Module *module;
  const rack::Param *knob;
  const rack::Input *cv;
  const rack::Param *av;
  const std::function<float(float)> function;
};
} // namespace DHE
