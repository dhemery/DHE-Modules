
#pragma once

#include <array>
#include <functional>
#include <utility>

#include "modules/components/Duration.h"
#include "util/range.h"
#include "util/sigmoid.h"

namespace DHE {

namespace Duration {

class Control {
public:
  explicit Control(std::function<float()> rotation,
                   std::function<Range const *()> range)
      : rotation{std::move(rotation)}, range{std::move(range)} {}

  auto seconds() -> float {
    auto const tapered = Sigmoid::j_shape.taper(rotation(), knobTaperCurvature);
    return range()->scale(tapered);
  }

private:
  const std::function<float()> rotation;
  const std::function<Range const *()> range;
};
} // namespace Duration
} // namespace DHE
