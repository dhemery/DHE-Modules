#pragma once

#include <array>
#include <ostream>
#include <string>

namespace dhe {
namespace curve_sequencer {
enum class GenerateMode { Curve, Hold, Sustain, Input, Chase, Level };

namespace generate_mode {
static auto constexpr size = 6;
static auto constexpr type_name = "GenerateMode";
static auto constexpr values = std::array<GenerateMode, size>{
    GenerateMode::Curve, GenerateMode::Hold,  GenerateMode::Sustain,
    GenerateMode::Input, GenerateMode::Chase, GenerateMode::Level,
};

static auto constexpr labels = std::array<char const *, size>{
    "Curve", "Hold", "Sustain", "Input", "Chase", "Level"};

static inline auto label(GenerateMode mode) -> std::string {
  return labels[static_cast<size_t>(mode)];
}
} // namespace generate_mode

struct GenerateModes {
  using ValueType = GenerateMode;
  static auto constexpr size = generate_mode::size;
  static auto constexpr slug = "generate-mode";

  static auto constexpr &labels = generate_mode::labels;
};

static inline auto operator<<(std::ostream &os, GenerateMode mode)
    -> std::ostream & {
  auto const index = static_cast<size_t>(mode);
  if (index >= generate_mode::size) {
    return os << "Unknown " << generate_mode::type_name << ' ' << index;
  }
  return os << generate_mode::label(mode);
}

} // namespace curve_sequencer
} // namespace dhe
