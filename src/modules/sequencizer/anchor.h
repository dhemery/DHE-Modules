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

template <typename Module> class Anchor {
public:
  Anchor(Module &module, AnchorType type) : module_{module}, type_{type} {}

  void enter(int step) {
    step_ = step;
    sample_ = source_voltage();
  }

  auto voltage() const -> float {
    auto const mode = module_.anchor_mode(type_, step_);
    return mode == AnchorMode::Track ? source_voltage() : sample_;
  }

private:
  Module &module_{};
  AnchorType type_{};
  int step_{};
  float sample_{};

  auto source_voltage() const -> float {
    auto const multiplier = module_.anchor_multiplier(type_, step_);
    switch (module_.anchor_source(type_, step_)) {
    case AnchorSource::InA:
      return module_.in_a() * multiplier;
    case AnchorSource::InB:
      return module_.in_b() * multiplier;
    case AnchorSource::InC:
      return module_.in_c() * multiplier;
    case AnchorSource::Level:
      return module_.level() * multiplier;
    case AnchorSource::Out:
      return module_.output() * multiplier;
    default:
      return 0.F;
    }
  }
};
} // namespace sequencizer
} // namespace dhe
