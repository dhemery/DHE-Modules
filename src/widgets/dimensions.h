#pragma once

namespace dhe {

static auto constexpr mm_per_in = 25.4F;
static auto constexpr px_per_in = 75.F;
static auto constexpr px_per_mm = px_per_in / mm_per_in;

static inline auto constexpr mm2px(float mm) -> float { return mm * px_per_mm; }

static auto constexpr button_radius = 3.F;
static auto constexpr button_diameter = button_radius * 2.F;

static auto constexpr light_radius = 1.088F;
static auto constexpr light_diameter = light_radius * 2.F;

static auto constexpr port_radius = 4.2F;
static auto constexpr port_diameter = port_radius * 2.F;

static auto constexpr small_knob_radius = 4.2F;
static auto constexpr small_knob_diameter = small_knob_radius * 2.F;

static auto constexpr small_label_size = 7.0F / px_per_mm;

} // namespace dhe
