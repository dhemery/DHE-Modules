#pragma once

#include <utility>

#include <engine.hpp>

#include "util/range.h"

namespace DHE {

class Knob {
public:
  template <typename Supplier>
  explicit Knob(const Supplier &supplier) : supplier{supplier} {}

  static auto plain(const rack::Module *module, int knob_index) -> Knob {
    const auto *knob_param = &module->params[knob_index];
    return Knob{[knob_param] { return knob_param->value; }};
  }

  static auto with_cv(const rack::Module *module, int knob_index, int cv_index)
      -> Knob {
    const auto *knob_param = &module->params[knob_index];
    const auto modulate = modulated_by(module, cv_index);
    return Knob{[knob_param, modulate] { return modulate(knob_param->value); }};
  }

  static auto with_av(const rack::Module *module, int knob_index, int cv_index,
                      int av_index) -> Knob {
    const auto *knob_param = &module->params[knob_index];
    const auto modulate = modulated_by(module, cv_index, av_index);
    return Knob{[knob_param, modulate] { return modulate(knob_param->value); }};
  }

  auto operator()() const -> float { return supplier(); }

private:
  const std::function<float()> supplier;

  static auto modulate(float rotation, float cv = 0.f, float av = 1.f)
      -> float {
    static constexpr auto av_range = Range{-1.f, 1.f};
    static constexpr auto cv_to_offset = 0.1f;

    auto offset = cv * cv_to_offset;
    auto multipler = av_range.scale(av);
    return rotation + multipler * offset;
  }

  static auto modulated_by(const rack::Module *module, int cv_index)
      -> const std::function<float(float)> {
    const auto *cv_input = &module->inputs[cv_index];
    return [cv_input](float rotation) -> float {
      return modulate(rotation, cv_input->value);
    };
  }

  static auto modulated_by(const rack::Module *module, int cv_index,
                           int av_index) -> const std::function<float(float)> {
    const auto *cv_input = &module->inputs[cv_index];
    const auto *av_param = &module->params[av_index];
    return [cv_input, av_param](float rotation) -> float {
      return modulate(rotation, cv_input->value, av_param->value);
    };
  }
};
} // namespace DHE
