#pragma once

namespace dhe {
namespace curve_sequencer {
enum class AnchorType { Start, End };
static auto constexpr anchor_type_count = static_cast<int>(AnchorType::End) + 1;

enum class AnchorMode { Snap, Track };
static auto constexpr anchor_mode_count =
    static_cast<int>(AnchorMode::Track) + 1;

enum class AnchorSource { Level, In, Out };
static auto constexpr anchor_source_count =
    static_cast<int>(AnchorSource::Out) + 1;

template <typename M> class Anchor {
public:
  Anchor(M &module, AnchorType type) : module_{module}, type_{type} {}

  void enter(int step) {
    step_ = step;
    snapshot_ = source_voltage();
  }

  auto voltage() const -> float {
    auto const mode = module_.anchor_mode(type_, step_);
    return mode == AnchorMode::Track ? source_voltage() : snapshot_;
  }

private:
  M &module_{};
  AnchorType type_{};
  int step_{};
  float snapshot_{};

  auto source_voltage() const -> float {
    switch (module_.anchor_source(type_, step_)) {
    case AnchorSource::In:
      return module_.input();
    case AnchorSource::Level:
      return module_.anchor_level(type_, step_);
    case AnchorSource::Out:
      return module_.output();
    default:
      return 0.F;
    }
  }
};
} // namespace curve_sequencer
} // namespace dhe
