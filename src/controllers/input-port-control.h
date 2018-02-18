#pragma once

#include <functional>
#include <util/interval.h>
#include <algorithm>
#include <utility>

namespace DHE {
static constexpr auto DEFAULT_INPUT_PORT_BUTTON = [] { return 0.f; };

struct InputPortControl {
  explicit InputPortControl(std::function<float()> input)
      : InputPortControl{std::move(input), DEFAULT_INPUT_PORT_BUTTON, [](float) {}, UNIPOLAR_CV} {}

  InputPortControl(std::function<float()> port, std::function<float()> button, std::function<void(float)> light)
      : InputPortControl(std::move(port), std::move(button), std::move(light), UNIPOLAR_CV) {}

  InputPortControl(std::function<float()> port, std::function<float()> button, std::function<void(float)> light, Interval range)
      : input{std::move(port)},
        button{std::move(button)},
        light{std::move(light)},
        range{range} {}

  float operator()() const {
    float scaled_button = range.scale(button());
    light(scaled_button);
    return range.clamp(std::max(input(), scaled_button));
  }

  const std::function<float()> input;
  const std::function<float()> button;
  const std::function<void(float)> light;
  const Interval range;
};

}
