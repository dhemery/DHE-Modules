#pragma once

#include <string>

namespace dhe {
class Latch {
public:
  constexpr Latch() = default;
  constexpr Latch(bool state, bool edge) : state_{state}, edge_{edge} {}
  //  Latch(Latch const &) = default;
  //  auto operator=(Latch const &) -> Latch& = default;
  void clock(bool signal) {
    edge_ = signal != state_;
    state_ = signal;
  }

  auto is_high() const -> bool { return state_; };
  auto is_edge() const -> bool { return edge_; };

  auto is_low() const -> bool { return !is_high(); };
  auto is_fall() const -> bool { return is_edge() && is_low(); };
  auto is_rise() const -> bool { return is_edge() && is_high(); };

  friend auto operator==(Latch const &left, Latch const &right) -> bool {
    return left.state_ == right.state_ && left.edge_ == right.edge_;
  }

  auto str() const -> std::string {
    std::string state_str{state_ ? "true" : "false"};
    std::string edge_str{edge_ ? "true" : "false"};
    return std::string{"Latch{"} + state_str + "," + edge_str + "}";
  }

private:
  bool state_{};
  bool edge_{};
};

} // namespace dhe
