#pragma once

#include <array>
#include <ostream>

namespace dhe {
namespace sequencizer {

enum class AnchorMode { Sample, Track };

namespace anchor_mode {
static auto constexpr size = 2;
static auto constexpr labels =
    std::array<char const *, size>{"Sample the source", "Track the source"};
} // namespace anchor_mode

struct AnchorModes {
  using ValueType = AnchorMode;
  static auto constexpr &labels = anchor_mode::labels;
  static auto constexpr size = anchor_mode::size;
  static auto constexpr stepper_slug = "anchor-mode";
};

enum class AnchorSource {
  Level,
  InA,
  InB,
  InC,
  Out,
};

namespace anchor_source {
static auto constexpr size = 5;
static auto constexpr labels = std::array<char const *, size>{
    "Level", "A", "B", "C", "Out",
};

static inline auto label(AnchorSource s) -> char const * {
  return labels[static_cast<size_t>(s)];
}
static auto constexpr values = std::array<AnchorSource, size>{
    AnchorSource::Level, AnchorSource::InA, AnchorSource::InB,
    AnchorSource::InC,   AnchorSource::Out,
};
} // namespace anchor_source

struct AnchorSources {
  using ValueType = AnchorSource;
  static auto constexpr &labels = anchor_source::labels;
  static auto constexpr size = anchor_source::size;
  static auto constexpr stepper_slug = "anchor-source";
};

static inline auto operator<<(std::ostream &os, AnchorSource s)
    -> std::ostream & {
  return os << "AnchorSource::" << anchor_source::label(s);
}

enum class AnchorType { Start, End };

namespace anchor_type {
static auto constexpr labels = std::array<char const *, 2>{
    "Start",
    "End",
};

static inline auto label(AnchorType type) -> char const * {
  return labels[static_cast<size_t>(type)];
}
} // namespace anchor_type

template <typename Signals> struct Anchor {
  Anchor(Signals &signals, AnchorType type) : signals_{signals}, type_{type} {}

  void enter(int step) {
    step_ = step;
    sample_ = source_voltage();
  }

  auto voltage() const -> float {
    auto const mode = signals_.anchor_mode(type_, step_);
    return mode == AnchorMode::Track ? source_voltage() : sample_;
  }

private:
  Signals &signals_{};
  AnchorType type_{};
  int step_{};
  float sample_{};

  auto source_voltage() const -> float {
    switch (signals_.anchor_source(type_, step_)) {
    case AnchorSource::InA:
      return signals_.in_a();
    case AnchorSource::InB:
      return signals_.in_b();
    case AnchorSource::InC:
      return signals_.in_c();
    case AnchorSource::Level:
      return signals_.anchor_level(type_, step_);
    case AnchorSource::Out:
      return signals_.output();
    default:
      return 0.F;
    }
  }
};
} // namespace sequencizer
} // namespace dhe
