#pragma once

#include <array>
#include <string>
#include <vector>

namespace dhe {
namespace scannibal {
enum class AnchorMode { Sample, Track };

namespace anchor_mode {
static auto constexpr size = 2;
static auto constexpr labels =
    std::array<char const *, size>{"Sample the source", "Track the source"};
} // namespace anchor_mode

struct AnchorModes {
  using ValueType = AnchorMode;
  static auto constexpr size = anchor_mode::size;
  static auto constexpr stepper_slug = "anchor-mode";

  static inline auto labels() -> std::vector<std::string> {
    return {anchor_mode::labels.cbegin(), anchor_mode::labels.cend()};
  }
};

enum class AnchorSource { Level, InA, InB, InC, Out };

namespace anchor_source {
static auto constexpr size = 5;

static auto constexpr labels =
    std::array<char const *, size>{"Level", "A", "B", "C", "Out"};

static auto constexpr names = std::array<char const *, size>{
    "AnchorSource::Level", "AnchorSource::InA", "AnchorSource::InB",
    "AnchorSource::InC",   "AnchorSource::Out",
};

static inline auto name(AnchorSource s) -> char const * {
  return names[static_cast<size_t>(s)];
}

static auto constexpr values = std::array<AnchorSource, size>{
    AnchorSource::Level, AnchorSource::InA, AnchorSource::InB,
    AnchorSource::InC,   AnchorSource::Out,
};
} // namespace anchor_source

struct AnchorSources {
  using ValueType = AnchorSource;
  static auto constexpr size = anchor_source::size;
  static constexpr auto stepper_slug = "anchor-source";
  static inline auto labels() -> std::vector<std::string> {
    return {anchor_source::labels.cbegin(), anchor_source::labels.cend()};
  }
};

enum class AnchorType { Phase0, Phase1 };

namespace anchor_type {
static auto constexpr size = 2;
}

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
    switch (module_.anchor_source(type_, step_)) {
    case AnchorSource::InA:
      return module_.in_a();
    case AnchorSource::InB:
      return module_.in_b();
    case AnchorSource::InC:
      return module_.in_c();
    case AnchorSource::Level:
      return module_.anchor_level(type_, step_);
    case AnchorSource::Out:
      return module_.output();
    default:
      return 0.F;
    }
  }
};
} // namespace scannibal
} // namespace dhe
