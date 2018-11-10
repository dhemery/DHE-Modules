#pragma once

#include <engine.hpp>

#include "util/range.hpp"

namespace DHE {

struct Module : rack::Module {
  Module(int param_count, int input_count, int output_count)
      : rack::Module(param_count, input_count, output_count) {}

  float gate_button(int index) const;

  float input(int index) const { return inputs[index].value; }

  float param(int index) const { return params[index].value; }

  std::function<float()> knob(int rotation) const;

  std::function<float()> knob(int rotation, int cv) const;

  std::function<float()> knob(int rotation, int cv, int av) const;

  template <typename C>
  std::function<C const &()> choice(int switch_param,
                                    std::vector<C> choices) const {
    return [this, switch_param, choices]() -> C const & {
      auto index = static_cast<int>(param(switch_param));
      return choices[index];
    };
  }

  std::function<Range const &()> range_switch(int switch_param) const {
    static auto ranges = std::vector<Range>{{-5.f, 5.f}, {0.f, 10.f}};
    return choice(switch_param, ranges);
  }
};

} // namespace DHE
