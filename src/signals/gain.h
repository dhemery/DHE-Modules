#pragma once

#include "ranged.h"

namespace dhe {

struct AttenuatorBounds {
  static auto constexpr min = 0.F;
  static auto constexpr max = 1.F;
};

struct AttenuatorDisplayBounds {
  static auto constexpr min = 0.F;
  static auto constexpr max = 100.F;
};

struct AttenuatorKnobMap : ScaledKnobMap<LinearRange<AttenuatorDisplayBounds>> {
  static auto constexpr unit = "%";
  static auto constexpr default_rotation = 1.F;
};

struct Attenuator : LinearRange<AttenuatorBounds> {
  using KnobMap = AttenuatorKnobMap;
};

struct AttenuverterBounds {
  static auto constexpr min = -1.F;
  static auto constexpr max = 1.F;
};

struct AttenuverterDisplayBounds {
  static auto constexpr min = -100.F;
  static auto constexpr max = 100.F;
};

struct AttenuverterKnobMap
    : ScaledKnobMap<LinearRange<AttenuverterDisplayBounds>> {
  static auto constexpr unit = "%";
  static auto constexpr default_rotation = 0.F;
};

struct Attenuverter : LinearRange<AttenuverterBounds> {
  using KnobMap = AttenuverterKnobMap;
};

struct GainBounds {
  static auto constexpr min = 0.F;
  static auto constexpr max = 2.F;
};

struct GainDisplayBounds {
  static auto constexpr min = 0.F;
  static auto constexpr max = 200.F;
};

struct GainKnobMap : ScaledKnobMap<LinearRange<GainDisplayBounds>> {
  static auto constexpr unit = "%";
  static auto constexpr default_rotation = 1.F;
};

struct Gain : LinearRange<GainBounds> {
  using KnobMap = GainKnobMap;
};

using Percentage = Attenuator;
} // namespace dhe
