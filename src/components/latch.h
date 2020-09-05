#pragma once

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

} // namespace dhe
