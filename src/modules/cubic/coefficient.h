#pragma once

#include "components/range.h"
#include "signals/linear-signals.h"

namespace dhe {
namespace cubic {
namespace coefficient {
static auto constexpr range = Range{-2.F, 2.F};
} // namespace coefficient

struct Coefficient : LinearSignal<Coefficient> {
  static auto constexpr &range = coefficient::range;
};

} // namespace cubic
} // namespace dhe
