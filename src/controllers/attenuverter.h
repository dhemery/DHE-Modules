#pragma once

#include <functional>

#include "util/interval.h"

namespace DHE {

struct Attenuverter {
  Attenuverter(
      std::function<float()> input,
      std::function<float()> modulator)
      : input{std::move(input)},
        modulator{std::move(modulator)} {}

  float operator()() {
    return input() * BIPOLAR_NORMAL.scale(modulator());
  }

  const std::function<float()> input;
  const std::function<float()> modulator;
};

}
