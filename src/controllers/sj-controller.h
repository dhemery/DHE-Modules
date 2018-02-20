#pragma once

#include <utility>

#include "util/interval.h"
#include "shape-control.h"

namespace DHE {

struct SportController {
  explicit SportController(
      ShapeControl shape,
      std::function<float()> sport_in,
      std::function<void(float)> sport_out)
      : shape{shape},
        sport_in{std::move(sport_in)},
        sport_out{std::move(sport_out)} {}

  float clamp(float in) {
    return BIPOLAR_CV.clamp(in);
  }

  float normalize(float in) {
    return BIPOLAR_CV.normalize(in);
  }

  float scale(float normalized) {
    return BIPOLAR_CV.scale(normalized);
  }

  void step() {
    sport_out(scale(shape(normalize(clamp(sport_in())))));
  }

private:
  ShapeControl shape;
  std::function<float()> sport_in;
  std::function<void(float)> sport_out;
};

}