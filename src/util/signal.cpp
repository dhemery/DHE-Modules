#include <array>

#include "util/range.h"
#include "util/signal.h"

namespace DHE {
namespace Signal {

auto ranges() -> std::array<Range const *, 2> const & {
  static auto const ranges =
      std::array<Range const *, 2>{&bipolar_range, &unipolar_range};
  return ranges;
}

auto range(bool is_uni) -> Range const & {
  return is_uni ? unipolar_range : bipolar_range;
}
} // namespace Signal
} // namespace DHE
