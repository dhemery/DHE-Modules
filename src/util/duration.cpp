#include <array>
#include "util/duration.h"
#include "util/range.h"

namespace DHE {
namespace Duration {

auto ranges() -> std::array<Range const *, 3> const & {
  static auto const ranges =
      std::array<Range const *, 3>{&short_range, &medium_range, &long_range};
  return ranges;
}
} // namespace Duration
} // namespace DHE