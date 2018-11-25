#pragma once

#include <engine.hpp>

#include "range.h"

namespace DHE {
namespace Modulation {
static constexpr auto av_range = Range{-1.f, 1.f};
static constexpr auto cv_to_offset = 0.1f;

static auto of(const rack::Param &knob, const rack::Input &cv)
-> float {
  auto rotation = knob.value;
  auto cv_offset = cv.value*cv_to_offset;
  return rotation + cv_offset;
}

static auto of(const rack::Param &knob, const rack::Input &cv,
                      const rack::Param &av) -> float {
  auto rotation = knob.value;
  auto cv_offset = cv.value*cv_to_offset;
  auto cv_multiplier = av_range.scale(av.value);
  return cv_multiplier*cv_offset + rotation;
}
}
}