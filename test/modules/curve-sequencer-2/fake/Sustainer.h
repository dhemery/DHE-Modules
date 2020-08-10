#pragma once

#include "components/Latch.h"

#include <string>

namespace test {
namespace fake {
  using dhe::Latch;

  class Sustainer {
  public:
    std::function<bool(int, Latch const &)> isDone{
        [](int s, Latch const &l) -> bool { throw forbidden("Sustainer.isDone", s, l.isHigh()); }};
  };
} // namespace fake
} // namespace test
