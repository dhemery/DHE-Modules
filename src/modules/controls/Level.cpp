#include <engine/Module.hpp>

#include "modules/controls/Level.h"

namespace dhe {

namespace level {

const std::array<Range const *, 2> ranges{&bipolarRange, &unipolarRange};

auto range(float switchPosition) -> Range const * {
  return ranges[static_cast<int>(switchPosition)];
}

} // namespace level
} // namespace dhe
