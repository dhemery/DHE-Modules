#pragma once

#include <engine.hpp>

#include "util/range.hpp"
#include "util/controls.hpp"

namespace DHE {

struct Module : rack::Module {
  Module(int param_count, int input_count, int output_count)
      : rack::Module(param_count, input_count, output_count) {}

  auto button(int param) const -> std::function<bool()> {
    return Controls::bool_value_of(params[param]);
  };

  auto trigger(int input) const -> std::function<bool()> {
    return Controls::bool_value_of(inputs[input]);
  };

  auto input(int index) const -> float { return inputs[index].value; }

  auto param(int index) const -> float { return params[index].value; }

  auto knob(int param) const -> std::function<float()> {
    return Controls::float_value_of(params[param]);
  };

  auto knob(int rotation, int cv) const -> std::function<float()>;

  auto knob(int rotation, int cv, int av) const -> std::function<float()>;

  template <typename C>
  auto choice(int switch_param, C choice1, C choice2) const
      -> std::function<C const &()> {
    auto choices = std::vector<C>{choice1, choice2};
    return [this, switch_param, choices]() -> C const & {
      auto index = static_cast<int>(param(switch_param));
      return choices[index];
    };
  }

  auto range_switch(int switch_param) const -> std::function<Range const &()> {
    auto const unipolar = Range{0.f, 10.f};
    auto const bipolar = Range{-5.f, 5.f};
    return choice(switch_param, bipolar, unipolar);
  }
};

} // namespace DHE
