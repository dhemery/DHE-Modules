#pragma once

#include <functional>

#include "util/interval.h"

namespace DHE {

static constexpr auto ALWAYS_OFF = [] { return 0.f; };
static constexpr auto UNLIT = [](float f) {};

struct ButtonControl {
  ButtonControl(
      std::function<float()> button = ALWAYS_OFF,
      std::function<void(float)> light = UNLIT)
      : button{std::move(button)},
        light{std::move(light)} {}

  bool operator()() {
    auto is_on = button() > 0.5;
    light(UNIPOLAR_CV.scale(is_on));
    return is_on;
  }

  const std::function<float()> button;
  const std::function<void(float)> light;
};

}
