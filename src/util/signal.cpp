#include "util/signal.h"

namespace dhe {
namespace signal {

  auto ranges() -> std::array<Range const *, 2> const & {
    static auto const ranges = std::array<Range const *, 2>{&bipolarRange, &unipolarRange};
    return ranges;
  }

  auto range(bool isUni) -> Range const & { return isUni ? unipolarRange : bipolarRange; }
} // namespace signal
} // namespace dhe
