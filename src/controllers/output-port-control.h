#pragma once

#include <functional>
#include <util/interval.h>
#include <algorithm>
#include <utility>

namespace DHE {

struct OutputPortControl {
  explicit OutputPortControl(std::function<void(float)> output, ButtonControl button = {})
  : send{std::move(output)}, button{std::move(button)} {}

  void operator()(float f) {
    send(button() ? UNIPOLAR_CV.scale(true) : UNIPOLAR_CV.clamp(f));
  }

  const std::function<void(float)> send;
  ButtonControl button;
};

}
