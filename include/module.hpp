#pragma once

#include <engine.hpp>

#include "util/range.hpp"

namespace DHE {

struct Module : rack::Module {
  Module(int param_count, int input_count, int output_count)
      : rack::Module(param_count, input_count, output_count) {}

  static auto modulated(const rack::Param &knob, const rack::Input &cv)
      -> float {
    auto rotation = knob.value;
    auto cv_offset = cv.value * 0.1f;
    return rotation + cv_offset;
  }

  static auto modulated(const rack::Param &knob, const rack::Input &cv,
                        const rack::Param &av) -> float {
    static constexpr auto av_range = Range{-1.f, 1.f};
    auto rotation = knob.value;
    auto cv_offset = cv.value * 0.1f;
    auto cv_multiplier = av_range.scale(av.value);
    return cv_multiplier * cv_offset + rotation;
  }

  auto modulated(int knob, int cv) const -> float {
    return modulated(params[knob], inputs[cv]);
  }

  auto modulated(int knob, int cv, int av) const -> float {
    return modulated(params[knob], inputs[cv], params[av]);
  }
};

} // namespace DHE
