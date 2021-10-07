#pragma once

namespace dhe {
namespace sequencizer {
enum class AnchorType { Start, End };
static auto constexpr anchor_type_count = static_cast<int>(AnchorType::End) + 1;

enum class AnchorMode { Sample, Track };
static auto constexpr anchor_mode_count =
    static_cast<int>(AnchorMode::Track) + 1;

enum class AnchorSource {
  Level,
  InA,
  InB,
  InC,
  Out,
};
static auto constexpr anchor_source_count =
    static_cast<int>(AnchorSource::Out) + 1;

template <typename Signals> class Anchor {
public:
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
