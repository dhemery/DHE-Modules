#pragma once
namespace dhe {
namespace envelope {
namespace hostage {
struct Modes {
  using ValueType = int;
  static inline auto labels() -> std::vector<std::string> const & {
    static auto const labels = std::vector<std::string>{"Hold", "Sustain"};
    return labels;
  }
};

} // namespace hostage
} // namespace envelope
} // namespace dhe
