#pragma once

#include <engine.hpp>

#include "util/controls.hpp"
#include "util/range.hpp"

namespace DHE {

struct Module : rack::Module {
  Module(int param_count, int input_count, int output_count)
      : rack::Module(param_count, input_count, output_count) {}

  auto knob(int param) const -> std::function<float()> {
    return Controls::float_value_of(params[param]);
  };

  auto knob(int knob, int cv) const -> std::function<float()> {
    auto const &knob_param = params[knob];
    auto const &cv_input = inputs[cv];

    return [&knob_param, &cv_input]() -> float {
      auto rotation = knob_param.value;
      auto cv_offset = cv_input.value * 0.1f;
      return rotation + cv_offset;
    };
  }

  auto knob(int knob, int cv, int av) const -> std::function<float()> {
    static constexpr auto av_range = Range{-1.f,1.f};
    auto const &knob_param = params[knob];
    auto const &cv_input = inputs[cv];
    auto const &av_param = params[av];

    return [&knob_param, &cv_input, &av_param]() -> float {
      auto rotation = knob_param.value;
      auto cv_offset = cv_input.value * 0.1f;
      auto cv_multiplier = av_range.scale(av_param.value);
      return cv_multiplier * cv_offset + rotation;
    };
  }

  auto button(int param) const -> std::function<bool()> {
    return Controls::bool_value_of(params[param]);
  };

  auto selector(int param) const -> std::function<int()> {
    return Controls::int_value_of(params[param]);
  }

  auto signal(int input) const -> std::function<float()> {
    return Controls::float_value_of(inputs[input]);
  };

  auto trigger(int input) const -> std::function<bool()> {
    return Controls::bool_value_of(inputs[input]);
  };

  auto input(int index) const -> float { return inputs[index].value; }

  template<typename C>
  auto choice(int switch_param, C choice1, C choice2) const
  -> std::function<C const &()> {
    auto choices = std::vector<C>{choice1, choice2};
    auto selected = selector(switch_param);
    return [selected, choices]() -> C const & {
      return choices[selected()];
    };
  }

  auto signal_range(int switch_param) const -> std::function<Range const &()> {
    auto const unipolar = Range{0.f, 10.f};
    auto const bipolar = Range{-5.f, 5.f};
    return choice(switch_param, bipolar, unipolar);
  }
};

} // namespace DHE
