#pragma once

#include <engine.hpp>

#include "util/range.hpp"
#include "util/signal.hpp"

namespace DHE {

struct Module : rack::Module {
  Module(int param_count, int input_count, int output_count)
      : rack::Module(param_count, input_count, output_count) {}

  auto float_param(int param) const -> std::function<float()> {
    auto const &source{params[param]};
    return [&source]() -> float { return source.value; };
  }

  auto int_param(int param) const -> std::function<int()> {
    auto const &source{params[param]};
    return [&source]() -> int { return static_cast<int>(source.value); };
  }

  auto bool_param(int param) const -> std::function<bool()> {
    auto const &source{params[param]};
    return [&source]() -> bool { return source.value > 0.1f; };
  }

  auto float_in(int input) const -> std::function<float()> {
    auto const &source{inputs[input]};
    return [&source]() -> float { return source.value; };
  }

  auto bool_in(int input) const -> std::function<bool()> {
    auto const &source{inputs[input]};
    return [&source]() -> bool { return source.value > 0.1f; };
  }

  auto knob(int param) const -> std::function<float()> {
    return float_param(param);
  };

  auto knob(int knob, int cv) const -> std::function<float()> {
    auto const &knob_param{params[knob]};
    auto const &cv_input{inputs[cv]};

    return [&knob_param, &cv_input]() -> float {
      auto rotation{knob_param.value};
      auto cv_offset{cv_input.value * 0.1f};
      return rotation + cv_offset;
    };
  }

  auto knob(int knob, int cv, int av) const -> std::function<float()> {
    static auto constexpr av_range{Range{-1.f, 1.f}};
    auto const &knob_param{params[knob]};
    auto const &cv_input{inputs[cv]};
    auto const &av_param{params[av]};

    return [&knob_param, &cv_input, &av_param]() -> float {
      auto rotation = knob_param.value;
      auto cv_offset = cv_input.value * 0.1f;
      auto cv_multiplier = av_range.scale(av_param.value);
      return cv_multiplier * cv_offset + rotation;
    };
  }

  static auto modulated(rack::Param const &knob, rack::Input const &cv)
      -> float {
    auto const rotation = knob.value;
    auto const cv_offset = cv.value * 0.1f;
    return rotation + cv_offset;
  }

  static auto modulated(rack::Param const &knob, rack::Input const &cv,
                        rack::Param const &av) -> float {
    static auto constexpr av_range{Range{-1.f, 1.f}};
    auto const rotation = knob.value;
    auto const cv_offset = cv.value * 0.1f;
    auto const cv_multiplier = av_range.scale(av.value);
    return cv_multiplier * cv_offset + rotation;
  }

  auto modulated(int knob, int cv) const -> float {
    return modulated(params[knob], inputs[cv]);
  }

  auto modulated(int knob, int cv, int av) const -> float {
    return modulated(params[knob], inputs[cv], params[av]);
  }

  template <typename Selector, typename Choice>
  auto choice(Selector selector, Choice choice0, Choice choice1) const
      -> std::function<Choice const &()> {
    auto choices{std::vector<Choice>{choice0, choice1}};
    return
        [selector, choices]() -> Choice const & { return choices[selector()]; };
  }

  template <typename Selector>
  auto signal_range(Selector selector) const -> std::function<Range const &()> {
    return choice(selector, Signal::bipolar_range, Signal::bipolar_range);
  }
};

} // namespace DHE
