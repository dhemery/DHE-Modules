#pragma once

#include <engine.hpp>

#include "util/range.hpp"

namespace DHE {

struct Module : rack::Module {
  Module(int param_count, int input_count, int output_count)
      : rack::Module(param_count, input_count, output_count) {}

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
};

} // namespace DHE
