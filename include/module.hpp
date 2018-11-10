#pragma once

#include <engine.hpp>

#include "util/range.hpp"

namespace DHE {

struct Module : rack::Module {
  Module(int param_count, int input_count, int output_count)
      : rack::Module(param_count, input_count, output_count) {}

  auto gate_button(int index) const -> float;

  auto input(int index) const -> float { return inputs[index].value; }

  auto param(int index) const -> float { return params[index].value; }

  auto knob(int rotation) const -> std::function<float()>;

  auto knob(int rotation, int cv) const -> std::function<float()>;

  auto knob(int rotation, int cv, int av) const -> std::function<float()>;

  template <typename C>
  auto choice(int switch_param, std::vector<C> choices) const
      -> std::function<C const &()> {
    return [this, switch_param, choices]() -> C const & {
      auto index = static_cast<int>(param(switch_param));
      return choices[index];
    };
  }

  auto range_switch(int switch_param) const -> std::function<Range const &()> {
    static auto ranges = std::vector<Range>{{-5.f, 5.f}, {0.f, 10.f}};
    return choice(switch_param, ranges);
  }
};

} // namespace DHE
