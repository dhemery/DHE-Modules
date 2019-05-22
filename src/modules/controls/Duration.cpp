#include <engine/Module.hpp>

#include "modules/controls/Duration.h"
#include "util/sigmoid.h"

namespace dhe {

namespace duration {

const float knobTaperCurvature = 0.8018017;

const std::array<Range const *, 3> ranges{&shortRange, &mediumRange,
                                          &longRange};

auto range(float switchPosition) -> Range const * {
  return ranges[static_cast<int>(switchPosition)];
}

auto knobTaper() -> std::function<float(float)> {
  return [](float rotation) -> float {
    return sigmoid::j_taper(rotation, knobTaperCurvature);
  };
}
} // namespace duration
} // namespace dhe
