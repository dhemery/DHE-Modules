#include <array>

#include "modules/controls/Level.h"
#include "util/range.h"

namespace DHE {

const Range Level::unipolar_range{0.f, 10.f};
const Range Level::bipolar_range{-5.f, 5.f};

const std::array<Range const *, 2> Level::ranges{&bipolar_range,
                                                 &unipolar_range};

ConstantParam Level::defaultRangeSwitch{1.f};

auto Level::voltage() -> float {
  auto const rangeChoice = static_cast<int>(rangeSwitch.getValue());
  auto const *range = ranges[rangeChoice];
  auto const rotation = knob.getValue();
  return range->scale(rotation);
}

} // namespace DHE
