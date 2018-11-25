#pragma once

#include <utility>
#include <engine.hpp>
#include "range.h"

namespace DHE {

class Knob {
public:
  static auto plain(const rack::Module *module, int knob) -> Knob {
    auto supplier = [module, knob] { return module->params[knob].value; };
    return Knob{supplier};
  }

  static auto modulated(const rack::Module *module, int knob, int cv) -> Knob {
    auto supplier = [module, knob, cv] {
      auto rotation = module->params[knob].value;
      auto offset = module->inputs[cv].value*cv_to_offset;
      return rotation + offset;
    };
    return Knob{supplier};
  }

  static auto modulated(const rack::Module *module, int knob, int cv, int av) -> Knob {
    auto supplier = [module, knob, cv, av] {
      auto rotation = module->params[knob].value;
      auto offset = module->inputs[cv].value*cv_to_offset;
      auto multipler = av_range.scale(module->params[av].value);
      return rotation + multipler*offset;
    };
    return Knob{supplier};
  }

  auto operator()() const -> float {
    return supplier();
  }

private:
  explicit Knob(std::function<float()> supplier) : supplier{std::move(supplier)} {}

  static constexpr auto av_range = Range{-1.f, 1.f};
  static constexpr auto cv_to_offset = 0.1f;
  const std::function<float()> supplier;
};
}