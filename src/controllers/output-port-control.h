#pragma once

#include <functional>
#include <util/interval.h>
#include <algorithm>
#include <utility>

namespace DHE {
static constexpr auto DEFAULT_OUTPUT_PORT_BUTTON = [] { return 0.f; };

struct OutputPortControl {
  explicit OutputPortControl(std::function<void(float)> output)
  : OutputPortControl{std::move(output), DEFAULT_OUTPUT_PORT_BUTTON, [](float) {}, UNIPOLAR_CV} {}

  OutputPortControl(std::function<void(float)> output, std::function<float()> button, std::function<void(float)> light)
      : OutputPortControl{std::move(output), std::move(button), std::move(light), UNIPOLAR_CV} {}

  OutputPortControl(std::function<void(float)> output, std::function<float()> button, std::function<void(float)> light, Interval range)
      : send{std::move(output)},
        button{std::move(button)},
        light{std::move(light)},
        range{range} {}

  void operator()(float f) {
    float scaled_button = range.scale(button());
    light(scaled_button);
    send(range.clamp(std::max(f, scaled_button)));
  }

  const std::function<void(float)> send;
  const std::function<float()> button;
  const std::function<void(float)> light;
  const Interval range;
};

}
