#pragma once

#include <iostream>
#include <sstream>
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

  auto is_high() const -> bool { return state_; };
  auto is_edge() const -> bool { return edge_; };

  auto is_low() const -> bool { return !is_high(); };
  auto is_fall() const -> bool { return is_edge() && is_low(); };
  auto is_rise() const -> bool { return is_edge() && is_high(); };

  auto operator==(Latch const &rhs) const -> bool {
    return state_ == rhs.state_ && edge_ == rhs.edge_;
  }
  auto operator!=(Latch const &rhs) const -> bool { return !(rhs == *this); }

  friend auto operator<<(std::ostream &os, Latch const &latch)
      -> std::ostream & {
    os << "Latch{" << latch.state_ << ',' << latch.edge_ << '}';
    return os;
  }

  auto str() const -> std::string {
    auto os = std::ostringstream{};
    os << std::boolalpha << *this;
    return os.str();
  }

private:
  bool state_{};
  bool edge_{};
};

} // namespace dhe
