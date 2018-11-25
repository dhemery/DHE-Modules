#pragma once

#include <utility>
#include <engine.hpp>
#include "range.h"

namespace DHE {

class Knob {
public:
  static auto plain(const rack::Module *module, int knob) -> Knob {
    const auto *knob_param = &module->params[knob];
    return Knob{[knob_param] { return knob_param->value; }};
  }

  static auto modulated(const rack::Module *module, int knob, int cv) -> Knob {
    const auto *knob_param = &module->params[knob];
    const auto *cv_input = &module->inputs[cv];
    return Knob{[knob_param, cv_input] {
      return modulated(knob_param->value, cv_input->value);
    }};
  }

  static auto modulated(const rack::Module *module, int knob, int cv, int av) -> Knob {
    const auto *knob_param = &module->params[knob];
    const auto *cv_input = &module->inputs[cv];
    const auto *av_param = &module->params[av];
    return Knob{[knob_param, cv_input, av_param] {
      return modulated(knob_param->value, cv_input->value, av_param->value);
    }};
  }

  static auto modulated(float knob, float cv = 0.f, float av = 1.f) -> float {
    static constexpr auto av_range = Range{-1.f, 1.f};
    static constexpr auto cv_to_offset = 0.1f;

    auto offset = cv*cv_to_offset;
    auto multipler = av_range.scale(av);
    return knob + multipler*offset;
  }

  auto operator()() const -> float {
    return supplier();
  }

private:
  explicit Knob(std::function<float()> supplier) : supplier{std::move(supplier)} {}

  const std::function<float()> supplier;
};
}