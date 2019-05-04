#include "util/sigmoid.h"

namespace DHE {

namespace Sigmoid {

auto shapes() -> std::array<Shape const *, 2> const & {
  static auto const ranges = std::array<Shape const *, 2>{&j_shape, &s_shape};
  return ranges;
}
} // namespace Sigmoid
} // namespace DHE
