#pragma once

#include <functional>
#include <util/interval.h>
#include <algorithm>
#include <utility>
#include "button-control.h"

namespace DHE {

struct OutputPortControl {
  explicit OutputPortControl(std::function<void(float)> output, std::function<bool()> button = [] { return false; })
      : send{std::move(output)}, button{std::move(button)} {}

  void operator()(float f) {
    send(button() ? UNIPOLAR_CV.scale(true) : UNIPOLAR_CV.clamp(f));
  }

  const std::function<void(float)> send;
  const std::function<bool()> button;
};

}
