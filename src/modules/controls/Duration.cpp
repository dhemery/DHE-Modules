#include "modules/controls/Duration.h"
#include "util/sigmoid.h"

namespace DHE {

namespace Duration {

const float knobTaperCurvature = 0.8018017;

const std::array<Range const *, 3> ranges{&shortRange, &mediumRange,
                                          &longRange};

auto range(int switchPosition) -> Range const * {
  return ranges[static_cast<int>(switchPosition)];
}

auto from(const std::function<float()> &rotation,
          const std::function<Range const *()> &range)
    -> std::function<float()> {
  return [rotation, range]() -> float {
    auto const tapered = Sigmoid::j_shape.taper(rotation(), knobTaperCurvature);
    return range()->scale(tapered);
  };
}

auto from(const std::function<float()> &rotation) -> std::function<float()> {
  return [rotation]() -> float {
    auto const tapered = Sigmoid::j_shape.taper(rotation(), knobTaperCurvature);
    return mediumRange.scale(tapered);
  };
}
} // namespace Duration
} // namespace DHE
