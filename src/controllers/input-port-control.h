#pragma once

#include <functional>
#include <util/interval.h>
#include <algorithm>
#include <utility>

namespace DHE {
static constexpr auto DEFAULT_INPUT_PORT_BUTTON = [] { return 0.f; };

struct InputPortControl {
  InputPortControl(std::function<float()> port)
      : port{port},
        button{DEFAULT_INPUT_PORT_BUTTON},
        light{[](float) {}},
        range{UNIPOLAR_CV} {}

  InputPortControl(std::function<float()> port, std::function<float()> button, std::function<void(float)> light)
      : port{std::move(port)},
        button{std::move(button)},
        light{std::move(light)},
        range{UNIPOLAR_CV} {}

  InputPortControl(std::function<float()> port, std::function<float()> button, std::function<void(float)> light, Interval range)
      : port{std::move(port)},
        button{std::move(button)},
        light{std::move(light)},
        range{range} {}

  float operator()() const {
    float scaled_button = range.scale(button());
    light(scaled_button);
    return range.clamp(std::max(port(), scaled_button));
  }

  const std::function<float()> port;
  const std::function<float()> button;
  const std::function<void(float)> light;
  const Interval range;
};

}
