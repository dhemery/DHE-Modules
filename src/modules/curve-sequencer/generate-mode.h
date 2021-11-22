#pragma once

#include <array>
#include <ostream>
#include <string>
#include <vector>

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

static auto constexpr names = std::array<char const *, size>{
    "Curve", "Hold", "Sustain", "Input", "Chase", "Level"};

static inline auto name(GenerateMode mode) -> std::string {
  return names[static_cast<size_t>(mode)];
}
} // namespace generate_mode

struct GenerateModes {
  using value_type = GenerateMode;
  static auto constexpr size = generate_mode::size;
  static auto constexpr stepper_slug = "generate-mode";

  static inline auto labels() -> std::vector<std::string> {
    return {generate_mode::names.cbegin(), generate_mode::names.cend()};
  }
};

static inline auto operator<<(std::ostream &os, GenerateMode mode)
    -> std::ostream & {
  auto const index = static_cast<size_t>(mode);
  if (index >= generate_mode::size) {
    return os << "Unknown " << generate_mode::type_name << ' ' << index;
  }
  return os << generate_mode::name(mode);
}

} // namespace curve_sequencer
} // namespace dhe
