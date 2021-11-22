#pragma once

#include <array>
#include <ostream>
#include <string>

namespace dhe {

class Latch {
public:
  constexpr Latch() = default;
  constexpr Latch(bool state, bool edge) : state_{state}, edge_{edge} {}

  void clock(bool signal) {
    edge_ = signal != state_;
    state_ = signal;
  }

  constexpr auto is_high() const -> bool { return state_; };
  constexpr auto is_edge() const -> bool { return edge_; };

  constexpr auto is_low() const -> bool { return !is_high(); };
  constexpr auto is_fall() const -> bool { return is_edge() && is_low(); };
  constexpr auto is_rise() const -> bool { return is_edge() && is_high(); };

  constexpr auto operator==(Latch const &rhs) const -> bool {
    return state_ == rhs.state_ && edge_ == rhs.edge_;
  }
  constexpr auto operator!=(Latch const &rhs) const -> bool {
    return !(rhs == *this);
  }

private:
  bool state_{};
  bool edge_{};
};

namespace latch {
static auto constexpr rising = Latch{true, true};
static auto constexpr falling = Latch{false, true};
static auto constexpr high = Latch{true, false};
static auto constexpr low = Latch{false, false};

static auto constexpr values = std::array<Latch, 4>{low, high, falling, rising};

static auto constexpr name(Latch l) -> char const * {
  return l.is_high() ? (l.is_edge() ? "Rising" : "High")
                     : (l.is_edge() ? "Falling" : "Low");
}
} // namespace latch

static inline auto operator<<(std::ostream &os, Latch latch) -> std::ostream & {
  return os << latch::name(latch);
}

} // namespace dhe
