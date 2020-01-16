#include <utility>

#pragma once

namespace dhe {
class Latch {
public:
  constexpr Latch() = default;
  constexpr Latch(bool state, bool edge) : state{state}, edge{edge} {}

  void clock(bool signal) {
    edge = signal != state;
    state = signal;
  }

  auto isHigh() const -> bool { return state; };
  auto isEdge() const -> bool { return edge; };

  auto isLow() const -> bool { return !isHigh(); };
  auto isFall() const -> bool { return isEdge() && isLow(); };
  auto isRise() const -> bool { return isEdge() && isHigh(); };

  friend auto operator==(Latch const &left, Latch const &right) -> bool {
    return left.state == right.state && left.edge == right.edge;
  }

private:
  bool state{};
  bool edge{};
};
} // namespace dhe
