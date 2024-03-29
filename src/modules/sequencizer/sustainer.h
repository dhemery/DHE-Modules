#pragma once

#include "components/latch.h"

#include <array>
#include <ostream>

namespace dhe {
namespace sequencizer {

enum class SustainMode { No, Yes };

namespace sustain_mode {
static auto constexpr size = 2;
static auto constexpr labels =
    std::array<char const *, size>{"No sustain", "Sustain until triggered"};
static auto constexpr names = std::array<char const *, size>{
    "SustainMode::No",
    "SustainMode::Yes",
};
static inline auto name(SustainMode m) -> char const * {
  return names[static_cast<size_t>(m)];
}
static auto constexpr values = std::array<SustainMode, size>{
    SustainMode::No,
    SustainMode::Yes,
};
} // namespace sustain_mode

static inline auto operator<<(std::ostream &os, SustainMode m)
    -> std::ostream & {
  return os << sustain_mode::name(m);
}

struct SustainModes {
  using ValueType = SustainMode;
  static auto constexpr &labels = sustain_mode::labels;
  static auto constexpr size = sustain_mode::size;
  static auto constexpr slug = "sustain-mode";
};

template <typename Signals> class Sustainer {
public:
  Sustainer(Signals &signals) : signals_{signals} {}

  auto is_done(int step, Latch const &latch) -> bool {

    return (signals_.completion_mode(step) == SustainMode::No) ||
           is_triggered(signals_.trigger_mode(step), latch);
  }

private:
  Signals &signals_;
};

} // namespace sequencizer
} // namespace dhe
