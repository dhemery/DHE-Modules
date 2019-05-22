#include "modules/controls/Level.h"

namespace DHE {

namespace Level {

const std::array<Range const *, 2> ranges{&bipolarRange, &unipolarRange};

auto range(int switchPosition) -> Range const * {
  return ranges[static_cast<int>(switchPosition)];
}

auto from(const std::function<float()> &rotation,
          const std::function<Range const *()> &range)
    -> std::function<float()> {
  return [rotation, range]() -> float { return range()->scale(rotation()); };
}

auto from(const std::function<float()> &rotation, Range const &range)
    -> std::function<float()> {
  return [rotation, range]() -> float { return range.scale(rotation()); };
}
} // namespace Level
} // namespace DHE
