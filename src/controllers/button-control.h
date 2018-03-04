#pragma once

#include <functional>

#include "util/interval.h"

namespace DHE {

static constexpr auto ALWAYS_OFF = [] { return 0.f; };

struct ButtonControl {
  explicit ButtonControl(std::function<float()> button = ALWAYS_OFF) : button{std::move(button)}{}

  bool operator()() {
    return button() > 0.5;
  }

  const std::function<float()> button;
};

}
