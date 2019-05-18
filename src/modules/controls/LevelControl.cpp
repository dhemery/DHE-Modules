#include <array>

#include "modules/controls/LevelControl.h"
#include "util/range.h"

namespace DHE {

const Range LevelControl::unipolar_range{0.f, 10.f};
const Range LevelControl::bipolar_range{-5.f, 5.f};

const std::array<Range const *, 2> LevelControl::ranges{&bipolar_range,
                                                        &unipolar_range};

ConstantParam LevelControl::defaultRangeSwitch{1.f};

auto LevelControl::voltage() -> float { return range()->scale(rotation()); }

auto LevelControl::range() -> Range const * {
  auto const rangeChoice = static_cast<int>(rangeSwitch.getValue());
  return ranges[rangeChoice];
}

} // namespace DHE
