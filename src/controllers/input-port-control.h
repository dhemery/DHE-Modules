#pragma once

#include <functional>
#include <util/interval.h>
#include <algorithm>
#include <utility>
#include "button-control.h"

namespace DHE {

struct InputPortControl {
  explicit InputPortControl(std::function<float()> input, std::function<bool()> button = [] { return false; })
      : input{std::move(input)},
        button{std::move(button)} {}

  float operator()() {
    return button() ? UNIPOLAR_CV.scale(true) : UNIPOLAR_CV.clamp(input());
  }

  const std::function<float()> input;
  const std::function<bool()> button;
};

}
